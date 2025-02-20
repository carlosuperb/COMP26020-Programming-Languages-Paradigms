import sys
import re
import string
from collections import defaultdict

def parse_input(file_path):
    # Initialize a graph using defaultdict with sets to store neighbors
    graph = defaultdict(set)

    try:
        # Open the input file in read mode
        with open(file_path, 'r') as file:
            for line in file:
                # Skip empty lines
                if not line.strip():
                    continue
                
                # Remove leading/trailing whitespace and split by commas
                parts = line.strip().split(',')

                # Regular expression pattern to match only digits
                pattern = re.compile(r'^\d+$')

                # Check if all parts of the input line match the pattern
                if not all(pattern.match(part) for part in parts):
                    print("Contains invalid input format.")
                    sys.exit(1)

                node = int(parts[0])

                # Convert neighbors to a set to avoid duplicates
                neighbors = set()
                for neighbor in parts[1:]:  # Iterate over neighbors
                    neighbors.add(int(neighbor))  # Convert to int and add to set
                
                # Add neighbors to the current node
                graph[node].update(neighbors)

                # Ensure the graph is undirected by adding the current node as a neighbor to each neighbor
                for neighbor in neighbors:
                    graph[neighbor].add(node)

        return graph
    
    except FileNotFoundError:
        print(f"File not found: {file_path}")
        sys.exit(1)

def top_down_coloring(graph):
    # Create a list of available colors using uppercase letters A-Z (26 registers)
    colors = list(string.ascii_uppercase)

    # Sort nodes by number of neighbors in descending order (if it is same, sort by node number)
    sorted_nodes = sorted(graph.keys(), key=lambda x: (-len(graph[x]), x))

    # Dictionary to store the assigned color for each node
    assigned_colors = {}

    # Iterate over each node in the sorted list
    for current_node in sorted_nodes:
        # Initialize a set to store colors already used by neighboring nodes
        used_colors = set()

        # Check all neighbors of the current node and collect their assigned colors
        for neighbor in graph[current_node]:
            if neighbor in assigned_colors:
                used_colors.add(assigned_colors[neighbor])
        
        # Assign the first available color that hasn't been used by the neighbors
        for available_color in colors:
            if available_color not in used_colors:
                assigned_colors[current_node] = available_color
                break
        else:
            # If no available color is found (more than 26 colors needed), trigger an error and exit
            print("Error: registers are used out!")
            sys.exit(1)

    # Return the final color assignment for all nodes
    return assigned_colors

def write_output(file_path, assigned_color):
    with open(file_path, 'w') as file:
        for node in sorted(assigned_color.keys()):
            file.write(f"{node}{assigned_color[node]}\n")

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 <program_name> <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    graph = parse_input(input_file)
    assigned_color = top_down_coloring(graph)
    write_output(output_file, assigned_color)

    print(f"Coloring process successfully completed. Output saved at: {output_file}")

if __name__ == "__main__":
    main()
