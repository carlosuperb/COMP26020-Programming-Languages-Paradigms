#include "matrix.h"

#include <errno.h> /* for ENOSYS */

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

int matrix_allocate(matrix_t *m, int rows, int columns) {
    /* implement the function here ... */
    // Check for valid matrix pointer
    if (m == NULL) {
        return -1;  // Invalid matrix pointer
    }

    // Check for valid dimensions
    if (rows <= 0 || columns <= 0) {
        return -2;  //Invalid dimensions
    }

    // Set the dimensions of the matrix
    m->rows = rows;
    m->columns = columns;

    // Allocate memory for the row pointers
    m->content = (int **)malloc(rows * sizeof(int *));
    if (m->content == NULL) {
        return -3;  // Memory allocation for row pointers failed
    }

    // Allocate memory for each row
    for (int i = 0; i < rows; i++) {
        m->content[i] = (int*)malloc(columns * sizeof(int));
        if (m->content[i] == NULL) {
            // If allocation fails, free the allocated memory
            for (int j = 0; j < i; j++){
                free(m->content[j]);
            }
            free(m->content);
            m->content = NULL;  // Avoid dangling pointer
            return -4;  // Memory allocation failed
        }
    }

    return 0;   // Memory allocation successful
}

void matrix_free(matrix_t *m) {
    /* implement the function here ... */
    // Check if the matrix pointer is NULL
    if (m == NULL) {
        return; // Nothing to free
    }

    // Check if the matrix content is not NULL
    if (m->content != NULL) {
        // Free each row if rows and columns are valid
        if (m->rows > 0 && m->columns >0) {
            for (int i = 0; i < m->rows; i++) {
                free(m->content[i]);
            }
        }
        
        // Free the row pointer array
        free(m->content);

        // Set content to NULL to avoid dangling pointer
        m->content = NULL;
    }

    // Reset rows and columns to 0
    m->rows = 0;
    m->columns = 0;
}

void matrix_init_n(matrix_t *m, int n) {
    /* implement the function here ... */
    // Check if the matrix has been allocated
    if (m == NULL || m->content == NULL){
        return;  // Exit if the matrix is not allocated
    }

    if (m->rows <= 0 || m->columns <= 0){
        return; // Exit if matrix dimensions are not positive
    }

    // Set each element of the matrix to n
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            m->content[i][j] = n;
        }
    }
}

void matrix_init_zeros(matrix_t *m) {
    /* implement the function here ... */
    // Check if the matrix has been allocated
    if (m == NULL || m->content == NULL){
        return;  // Exit if the matrix is not allocated
    }

    // Check if matrix dimensions are valid
    if (m->rows <= 0 || m->columns <=0) {
        return; // Exit if dimensions are invalid
    }

    // Set each element of the matrix to 0
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            m->content[i][j] = 0;
        }
    }
}

int matrix_init_identity(matrix_t *m){
    /* implement the function here ... */
    // Check if the matrix has been allocated and is square
    if (m == NULL || m->content == NULL) {
        return -1;  // Return -1 if the matrix is not allocated
    }

    // Check if the matrix has valid dimensions
    if (m->rows <= 0 || m->columns <= 0) {
        return -2;  // Return -2 if the matrix has invalid dimensions
    }

    if (m->rows != m->columns) {
        return -3;  // Return -3 if the matrix is not sqaure
    }

    // Initialise the matrix as an identity matrix
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            if (i == j) {
                m->content[i][j] = 1;  // Set diagonal elements to 1
            } else {
                m->content[i][j] = 0;  // Set non-diagonal elements to 0
            }
        }
    }

    return 0;  // Successful initialisation
}

int matrix_init_rand(matrix_t *m, int val_min, int val_max) {
    /* implement the function here ... */
    // Check if the matrix has been allocated
    if (m == NULL || m->content == NULL){
        return -1;  // Return -1 if the matrix is not allocated
    }

    // Check if the matrix has valid dimensions
    if (m->rows <= 0 || m->columns <= 0) {
        return -2;  // Return -2 if the matrix has invalid dimensions
    }

    // Check if the range is valid
    if (val_min > val_max) {
        return -3;  // Return -3 for invalid range
    }

    // Generate random numbers and fill the matrix
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            // Generate a random integer within the range [val_min, val_max]
            m->content[i][j] = val_min + rand() % (val_max - val_min + 1);
        }
    }

    return 0;  // Successful initialisation
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
    /* implement the function here ... */
    // Check if both matrices are allocated
    if (m1 == NULL || m2 == NULL) {
        return 0;  // Return 0 if either matrix is not allocated
    }
    
    // Check if both matrices have valid content pointers
    if (m1->content == NULL || m2->content ==NULL) {
        return 0;   // Return 0 if either matrix has NULL content
    }

    // Check if both matrices have valid dimensions
    if (m1->rows <= 0 || m1->columns <= 0 || m2->rows <= 0 || m2->columns <= 0) {
        return 0;  // Return 0 if either matrix has invalid dimensions
    }

    // Check if both matrices have the same dimensions
    if (m1->rows != m2->rows || m1->columns != m2->columns) {
        return 0;  // Return 0 if matrices have different dimensions
    }
    
    //Compare each element of matrices
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->columns; j++){
            if (m1->content[i][j] != m2->content[i][j]) {
                return 0;  // Return 0 if matrices do not match
            }
        }
    }

    return 1;  // Return 1 if matrices match all elements
}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    // Check if any of the matrices are NULL
    if (m1 == NULL || m2 == NULL || result == NULL) {
        return -1;  // Return -1 if any matrix pointer is NULL
    }

    // Check if any matrices contents are NULL
    if (m1->content == NULL || m2->content == NULL) {
        return -2;  // Return -2 if either matrix content is NULL
    }

    // Check if dimensions are valid
    if (m1->rows <= 0 || m1->columns <= 0 || m2->rows <= 0 || m2->columns <= 0) {
        return -3;  // Return -3 if either matrix has invalid dimensions
    }
    
    // Check if both matrices have the same dimensions
    if (m1->rows != m2->rows || m1->columns != m2->columns) {
        return -4;  // Return -4 if matrices have different dimensions
    }

    // Allocate memory for result matrix
    result->rows = m1->rows;
    result->columns = m1->columns;
    if (matrix_allocate(result, result->rows, result->columns) != 0){
        return -5;  // Return -5 if memory allocation fails
    }

    // Perform matrix addition
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->columns; j++){
            result->content[i][j] = m1->content[i][j] + m2->content[i][j];
        }
    }

    return 0;  // Return 0 if success
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
    /* implement the function here ... */
    // Check if the matrix is valid
    if (m == NULL || m->content == NULL || result == NULL) {
        return -1;  // Return -1 if any matrix pointer is NULL
    }

    // Check if matrix dimensions are valid
    if (m->rows <= 0 || m->columns <= 0) {
        return -2;  // Return -2 if matrix has invalid dimensions
    }

    // Allocate memory for the result matrix
    result->rows = m->rows;
    result->columns = m->columns;
    if (matrix_allocate(result, result->rows, result->columns) != 0) {
        return -3;  // Return -1 if memory allocation fails
    }

    // Multiply each element of the matrix by the scalar
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            result->content[i][j] = m->content[i][j] * scalar;
        }
    }

    return 0;  // Return 0 if success
}

int matrix_transposition(matrix_t *m, matrix_t *result) {
    /* implement the function here ... */
    // Check if input matrices are valid
    if (m == NULL || result == NULL) {
        return -1;  // Return -1 if any matrix pointer is NULL
    }

    // Check if matrix content is allocated
    if (m->content == NULL) {
        return -2;  // Return -2 if the input matrix content is NULL
    }

    // Check if matrix dimensions are valid
    if (m->rows <= 0 || m->columns <= 0) {
        return -3;  // Return -3 if the input matrix has invalid dimensions
    }

    // Allocate memory for the tranposed matrix
    result->rows = m->columns;
    result->columns = m->rows;
    if (matrix_allocate(result, result->rows, result->columns) != 0) {
        return -4;  // Return -4 if memory allocation fails
    }

    // Transpose the matrix
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            result->content[j][i] = m->content[i][j];
        }
    }

    return 0;  // Return 0 if success
    
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    // Check if any input matrix pointers are NULL
    if (m1 == NULL || m2 == NULL || result == NULL) {
        return -1;  // Return -1 if any pointer is NULL
    }
    
    // Check if matrix content pointers are NULL
    if (m1->content == NULL || m2->content == NULL) {
        return -2;  // Return -2 if any matrix content is NULL
    }

    // Check if matrix dimensions are valid
    if (m1->rows <= 0 || m1->columns <= 0 || m2->rows <= 0 || m2->columns <= 0) {
        return -3;  // Return -3 if any matrix has invalid dimensions
    }

    // Check if the matrices can be multiplied
    if (m1->columns != m2->rows) {
        return -4;  // Return -4 if dimensions are incompatible for multiplication
    }

    // Allocate memory for the result matrix
    result->rows = m1->rows;
    result->columns = m2->columns;
    if (matrix_allocate(result, result->rows, result->columns) != 0) {
        return -5;  // Return -5 if memory allocation fails
    }

    // Perform matrix multiplication
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m2->columns; j++) {
            result->content[i][j] = 0;  // Initialize result element to 0
            for (int k = 0; k < m1->columns; k++) {
                result->content[i][j] += m1->content[i][k] * m2->content[k][j];
            }
        }
    }

    return 0;  // Return 0 if success
}

int matrix_dump_file(matrix_t *m, const char *output_file) {
    /* implement the function here ... */
    // Check if the matrix is allocated
    if (m == NULL || m->content == NULL) {
        return -1;  // Return -1 if matrix is not allocated
    }

    // Check file extension before attempting to open the file
    const char *extension = strrchr(output_file, '.');
    if (extension == NULL || strcmp(extension, ".txt") != 0) {
        return -2;  // Return -2 if the file is in an invalid format
    }

    // Open the file in write mode to overwrite if it exists
    FILE *file = fopen(output_file, "w");
    if (file == NULL) {
        return -3;  // Return -3 if the file could not be opened
    }

    // Write matrix content row by row in text format
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            if (j == m->columns - 1) {
                // Without space if last element in the row
                if (fprintf(file, "%d", m->content[i][j]) < 0) {
                    fclose(file);
                    return -4;  //Return -3 if matrix data could not be written
                }
            } else {
                // Add a space between elements
                if (fprintf(file, "%d ", m->content[i][j]) < 0) {
                    fclose(file);
                    return -4;  // Return -3 if matrix data could not be written
                }
            }
        }

        // Newline after each row
        if (fprintf(file, "\n") < 0) {
            fclose(file);
            return -5;  // Return -4 if newline could not be written
        }
    }

    fclose(file);   // Close the file after writing

    return 0;   // Return 0 if success
}

int matrix_allocate_and_init_file(matrix_t *m, const char *input_file) {
    /* implement the function here ... */
    // Check if the input file has a .txt extension
    const char *extension = strrchr(input_file, '.');
    if (extension == NULL || strcmp(extension, ".txt") != 0) {
        return -1;  // Return -1 if the file is in an invalid format
    }

    // Open the file in read mode
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        return -2;  // Return -2 if the file could not be opened
    }

    // Initialise counters
    int rows = 0;
    int columns = 0;
    int temp;
    int current_columns = 0;
    char line[1024];

    // Calculate rows and columns
    while (fgets(line, sizeof(line), file) != NULL) {
        // Trim leading and trailing spaces
        char *trimmed_line = strtok(line, "\n");
        if (trimmed_line == NULL || strlen(trimmed_line) == 0) {
            // Skip empty lines
            continue;
        }

        char *token = strtok(trimmed_line, " \t");
        current_columns = 0;
        while (token != NULL) {
            if (sscanf(token, "%d", &temp) != 1) {
                fclose(file);
                return -3;  //Return -3 if non-numeric data is found
            }
            current_columns++;
            token = strtok(NULL, " \t");
        }

        if (columns == 0) {
            columns = current_columns;  // Set columns based on the first valid row
        } else if (current_columns != columns) {
            fclose(file);
            return -4;  // Return -4 if inconsistent row length detected
        }
        rows++;

    }

    // Check if the file is empty or has incomplete data
    if (columns == 0 || rows == 0) {
        fclose(file);
        return -5;  // Return -5 if the file is empty or has incomplete data
    }

    // Set matrix dimensions
    m->rows = rows;
    m->columns = columns;

    // Allocate memory for the matrix
    if (matrix_allocate(m, m->rows, m->columns) != 0) {
        fclose(file);
        return -6;  // Close file and return -6 if memory allocation failed
    }

    // Reset file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Assign values to the matrix
    int row_index = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *trimmed_line = strtok(line, "\n");
        if (trimmed_line == NULL || strlen(trimmed_line) == 0) {
            continue;
        }

        char *token = strtok(trimmed_line, " \t");
        int col_index = 0;
        while (token != NULL) {
            if (sscanf(token, "%d", &(m->content[row_index][col_index])) != 1) {
                matrix_free(m); // Free allocated memory
                fclose(file);
                return -7;  // Return -7 if matrix data could not be read
            }
            col_index++;
            token = strtok(NULL, " \t");
        }
        row_index++;
    }

    fclose(file);  // Close the file after reading

    return 0;  // Return 0 if success
}
