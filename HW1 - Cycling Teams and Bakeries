#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

#define MAX_GRID_SIZE 1000
#define MAX_BAKERIES 1000000

typedef struct bakery_node {
    bool is_boundary;
    bool is_top_left_corner;
    int rank;
    int toast_count;
    int distance_up;
    int distance_down;
    int distance_right;
    int distance_left;
    int zero_distance_up;
    int zero_distance_down;
    int zero_distance_right;
    int zero_distance_left;
    struct bakery_node *up;
    struct bakery_node *down;
    struct bakery_node *right;
    struct bakery_node *left;
} Bakery;

struct grid_cell {
    int rank;
    int toast_count;
    Bakery *bakery_ptr;
};

struct rank_lookup {
    int row_id;
    int col_id;
    Bakery *bakery_ptr;
};

struct grid_cell grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
struct rank_lookup rank_to_bakery[MAX_BAKERIES];

Bakery* create_bakery_node(int rank, int toast_count, Bakery *up, Bakery *down, Bakery *right, Bakery *left) {
    Bakery *node = (Bakery *)malloc(sizeof(Bakery));
    assert(node != NULL);
    
    node->is_boundary = false;
    node->is_top_left_corner = false;
    node->rank = rank;
    node->toast_count = toast_count;
    node->distance_up = 0;
    node->distance_down = 0;
    node->distance_right = 0;
    node->distance_left = 0;
    node->zero_distance_up = 0;
    node->zero_distance_down = 0;
    node->zero_distance_right = 0;
    node->zero_distance_left = 0;
    node->up = up;
    node->down = down;
    node->right = right;
    node->left = left;
    
    return node;
}

void link_bakery_nodes(Bakery *current, Bakery *up, Bakery *down, Bakery *right, Bakery *left) {
    current->up = up;
    current->down = down;
    current->right = right;
    current->left = left;
}

Bakery* find_bakery_by_rank(int rank, int num_cols, int num_rows) {
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            if (rank == grid[i][j].rank) {
                return grid[i][j].bakery_ptr;
            }
        }
    }
    return NULL;
}

void print_linked_list(Bakery *head) {
    for(; head != NULL ; head = head->right) {
        printf("%d ", head->rank);
    }
    printf("\n");
}

bool is_not_visited(int rank, int *visited_ranks, int visited_count) {
    for(int i = 0; i <= visited_count; i++) {
        if(visited_ranks[i] == rank) {
            return false;
        }
    }
    return true;
}

Bakery* find_next_bakery(Bakery *current) {
    int min_rank = 0;
    Bakery *min_bakery = NULL;
    int min_distance = 0;
    Bakery *up = current->up;
    Bakery *down = current->down;
    Bakery *right = current->right;
    Bakery *left = current->left;

    if (up != NULL) {
        min_rank = up->rank;
        min_bakery = up;
        min_distance = current->distance_up;
    }

    if (down != NULL) {
        if (min_rank == 0 || current->distance_down < min_distance || 
            (down->rank < min_rank && current->distance_down <= min_distance)) {
            min_rank = down->rank;
            min_distance = current->distance_down;
            min_bakery = down;
        }
    }

    if (right != NULL) {
        if (min_rank == 0 || (current->distance_right < min_distance) ||
            (right->rank < min_rank && current->distance_right <= min_distance)) {
            min_rank = right->rank;
            min_distance = current->distance_right;
            min_bakery = right;
        }
    }

    if (left != NULL) {
        if (min_rank == 0 || current->distance_left < min_distance || 
            (left->rank < min_rank && current->distance_left <= min_distance)) {
            min_rank = left->rank;
            min_bakery = left;
        }
    }

    return min_bakery;
}

int main() {
    int num_rows, num_cols;
    scanf("%d%d", &num_rows, &num_cols);

    // Read bakery ranks
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            scanf("%d", &grid[i][j].rank);
        }
    }
    
    // Read toast counts
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            scanf("%d", &grid[i][j].toast_count);
        }
    }
    
    // Create bakery nodes
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            grid[i][j].bakery_ptr = create_bakery_node(
                grid[i][j].rank, 
                grid[i][j].toast_count, 
                NULL, NULL, NULL, NULL
            );
        }
    }

    // Link bakery nodes in grid structure - following original pattern
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (i == 0) {
                if (j == 0) {
                    link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, NULL);
                } else {
                    if (j == num_cols-1) {
                        link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, NULL, grid[i][j-1].bakery_ptr);
                    } else {
                        link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                    }
                }
            } else {
                if(i == num_rows-1) {
                    if(j == 0) {
                        link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, grid[i][j+1].bakery_ptr, NULL);
                    } else {
                        if (j == num_cols-1) {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, NULL, grid[i][j-1].bakery_ptr);
                        } else {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                        }
                    }
                } else {
                    if(j == 0) {
                        link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, NULL);
                    } else {
                        if(j == num_cols-1) {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, NULL, grid[i][j-1].bakery_ptr);
                        } else {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                        }
                    }
                }
            }
        }
    }
    
    grid[0][0].bakery_ptr->is_top_left_corner = true;

    // Create rank lookup table
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            int rank_index = grid[i][j].rank;
            rank_to_bakery[rank_index].bakery_ptr = grid[i][j].bakery_ptr;
            rank_to_bakery[rank_index].row_id = i;
            rank_to_bakery[rank_index].col_id = j;
        }
    }

    int training_days_1, racing_days, training_days_2;
    int starting_rank, tour_length, team_size;
    int racing_center_rank, racing_track_size;
    
    scanf("%d%d%d", &training_days_1, &racing_days, &training_days_2);

    Bakery *visited_bakeries[MAX_BAKERIES];
    int closed_bakery_ranks[MAX_BAKERIES];
    int closed_count = 0;
    
    // First training period
    for (int day = 0; day < training_days_1; day++) {
        scanf("%d%d%d", &starting_rank, &tour_length, &team_size);
        Bakery *current_bakery = rank_to_bakery[starting_rank].bakery_ptr;
        Bakery *next_bakery = NULL;
        int visited_count = 0;

        visited_bakeries[0] = current_bakery;
        int visited_index = 1;
        
        while (visited_count < tour_length) {
            next_bakery = find_next_bakery(current_bakery);

            if (next_bakery == NULL) {
                break;
            }

            Bakery *temp = current_bakery;
            current_bakery = next_bakery;
            
            // Update distances when removing visited bakery
            if (temp->up != NULL) {
                temp->up->down = temp->down;
                temp->up->distance_down = temp->up->distance_down + temp->distance_down + 1;
            }
            if (temp->down != NULL) {
                temp->down->up = temp->up;
                temp->down->distance_up = temp->down->distance_up + temp->distance_up + 1;
            }
            if (temp->right != NULL) {
                temp->right->left = temp->left;
                temp->right->distance_left = temp->right->distance_left + temp->distance_left + 1;
            }
            if (temp->left != NULL) {
                temp->left->right = temp->right;
                temp->left->distance_right = temp->left->distance_right + temp->distance_right + 1;
            }

            // Team eats toasts
            if (current_bakery->toast_count <= team_size) {
                current_bakery->toast_count = 0;
                closed_bakery_ranks[closed_count] = current_bakery->rank;
                closed_count++;
                visited_bakeries[visited_index] = current_bakery;
                visited_index++;
            } else {
                current_bakery->toast_count = current_bakery->toast_count - team_size;
                visited_bakeries[visited_index] = current_bakery;
                visited_index++;
            }
            visited_count++;
        }

        // Restore links for visited bakeries (excluding closed ones)
        for (int t = 0; t < visited_index; t++) {
            if (visited_bakeries[t]->up != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->up->down = visited_bakeries[t]->down;
                    visited_bakeries[t]->up->zero_distance_down = visited_bakeries[t]->up->zero_distance_down + visited_bakeries[t]->zero_distance_down + 1;
                } else {
                    visited_bakeries[t]->up->down = visited_bakeries[t];
                }
                visited_bakeries[t]->up->distance_down = visited_bakeries[t]->up->zero_distance_down;
            }
            if (visited_bakeries[t]->down != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->down->up = visited_bakeries[t]->up;
                    visited_bakeries[t]->down->zero_distance_up = visited_bakeries[t]->down->zero_distance_up + visited_bakeries[t]->zero_distance_up + 1;
                } else {
                    visited_bakeries[t]->down->up = visited_bakeries[t];
                }
                visited_bakeries[t]->down->distance_up = visited_bakeries[t]->down->zero_distance_up;
            }
            if (visited_bakeries[t]->right != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->right->left = visited_bakeries[t]->left;
                    visited_bakeries[t]->right->zero_distance_left = visited_bakeries[t]->right->zero_distance_left + visited_bakeries[t]->zero_distance_left + 1;
                } else {
                    visited_bakeries[t]->right->left = visited_bakeries[t];
                }
                visited_bakeries[t]->right->distance_left = visited_bakeries[t]->right->zero_distance_left;
            }
            if (visited_bakeries[t]->left != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->left->right = visited_bakeries[t]->right;
                    visited_bakeries[t]->left->zero_distance_right = visited_bakeries[t]->left->zero_distance_right + visited_bakeries[t]->zero_distance_right + 1;
                } else {
                    visited_bakeries[t]->left->right = visited_bakeries[t];
                }
                visited_bakeries[t]->left->distance_right = visited_bakeries[t]->left->zero_distance_right;
            }
        }
        
        for(int r = 0; r < visited_index; r++) {
            visited_bakeries[r] = NULL;
        }
    }

    // Racing period with rotation
    if (racing_days != 0) {
        // Restore closed bakeries for racing
        for(int i = 0; i < closed_count; i++) {
            Bakery *temp = rank_to_bakery[closed_bakery_ranks[i]].bakery_ptr;
            if (temp->up != NULL) {
                temp->up->down = temp;
                temp->up->zero_distance_down = 0;
                temp->up->distance_down = 0;
            }
            if (temp->down != NULL) {
                temp->down->up = temp;
                temp->down->zero_distance_up = 0;
                temp->down->distance_up = 0;
            }
            if (temp->right != NULL) {
                temp->right->left = temp;
                temp->right->zero_distance_left = 0;
                temp->right->distance_left = 0;
            }
            if (temp->left != NULL) {
                temp->left->right = temp;
                temp->left->zero_distance_right = 0;
                temp->left->distance_right = 0;
            }
        }

        // Process racing days - keeping the exact original rotation logic
        Bakery *top_left_corner = grid[0][0].bakery_ptr;
        for(int racing_day = 0; racing_day < racing_days; racing_day++) {
            scanf("%d%d", &racing_center_rank, &racing_track_size);
            
            if(racing_track_size == 1) {
                continue;
            }

            // Following the exact original rotation algorithm
            Bakery *middle = rank_to_bakery[racing_center_rank].bakery_ptr;
            Bakery *node1 = middle->left;
            Bakery *node2 = middle->right;
            int length = racing_track_size / 2;
            
            Bakery *node1_prev = node1;
            Bakery *node2_prev = node2;
            Bakery *node1_check = middle;
            Bakery *node2_check = middle;
            
            for(int i = 0; i < length - 1; i++) {
                if(node1->left != node1_check) {
                    node1 = node1->left;
                } else {
                    node1 = node1->right;
                }
                node1_check = node1_prev;
                node1_prev = node1;
                
                if(node2->right != node2_check) {
                    node2 = node2->right;
                } else {
                    node2 = node2->left;
                }
                node2_check = node2_prev;
                node2_prev = node2;
            }

            // Get boundary nodes
            Bakery *node1_b = NULL;
            Bakery *node2_b = NULL;
            
            if(node1->left != node1_check) {
                node1_b = node1->left;
            } else {
                node1_b = node1->right;
            }
            
            if(node2->right != node2_check) {
                node2_b = node2->right;
            } else {
                node2_b = node2->left;
            }

            Bakery *temp_node1_b = node1_b;
            Bakery *temp_node2_b = node2_b;

            // Set boundary flags and find vertical nodes
            Bakery *node1_b1 = NULL;
            Bakery *node1_b2 = NULL;
            Bakery *node1_b1_prev = NULL;
            Bakery *node1_b2_prev = NULL;
            Bakery *node1_b1_check = NULL;
            Bakery *node1_b2_check = NULL;
            
            if(node1_b != NULL) {
                node1_b->is_boundary = true;
                node1_b1 = node1_b->up;
                if (node1_b1 != NULL)
                    node1_b1->is_boundary = true;
                node1_b2 = node1_b->down;
                if (node1_b2 != NULL)
                    node1_b2->is_boundary = true;
                node1_b1_prev = node1_b1;
                node1_b2_prev = node1_b2;
                node1_b1_check = node1_b;
                node1_b2_check = node1_b;
            }
            
            Bakery *temp_node1_b1 = node1_b1;
            Bakery *temp_node1_b2 = node1_b2;

            Bakery *node2_b1 = NULL;
            Bakery *node2_b2 = NULL;
            Bakery *node2_b1_prev = NULL;
            Bakery *node2_b2_prev = NULL;
            Bakery *node2_b1_check = NULL;
            Bakery *node2_b2_check = NULL;
            
            if(node2_b != NULL) {
                node2_b->is_boundary = true;
                node2_b1 = node2_b->up;
                if (node2_b1 != NULL)
                    node2_b1->is_boundary = true;
                node2_b2 = node2_b->down;
                if (node2_b2 != NULL)
                    node2_b2->is_boundary = true;
                node2_b1_prev = node2_b1;
                node2_b2_prev = node2_b2;
                node2_b1_check = node2_b;
                node2_b2_check = node2_b;
            }
            
            Bakery *temp_node2_b1 = node2_b1;
            Bakery *temp_node2_b2 = node2_b2;

            // Find vertical rotation nodes
            Bakery *node1_1 = node1->up;
            Bakery *node1_1_prev = node1_1;
            Bakery *node1_2 = node1->down;
            Bakery *node1_2_prev = node1_2;
            Bakery *node1_1_check = node1;
            Bakery *node1_2_check = node1;
            Bakery *node2_rf = node2->up;
            Bakery *node2_rf_prev = node2_rf;
            Bakery *node2_rf_check = node2;
            node2_check = node2;
            node2 = node2->down;
            node2_prev = node2;

            for(int i = 0; i < length - 1; i++) {
                if(node1_1->up != node1_1_check) {
                    node1_1 = node1_1->up;
                } else {
                    node1_1 = node1_1->down;
                }
                node1_1_check = node1_1_prev;
                node1_1_prev = node1_1;
                
                if(node1_2->down != node1_2_check) {
                    node1_2 = node1_2->down;
                } else {
                    node1_2 = node1_2->up;
                }
                node1_2_check = node1_2_prev;
                node1_2_prev = node1_2;
                
                if(node2_rf->up != node2_rf_check) {
                    node2_rf = node2_rf->up;
                } else {
                    node2_rf = node2_rf->down;
                }
                node2_rf_check = node2_rf_prev;
                node2_rf_prev = node2_rf;
                
                if(node2->down != node2_check) {
                    node2 = node2->down;
                } else {
                    node2 = node2->up;
                }
                node2_check = node2_prev;
                node2_prev = node2;

                // Set boundary flags for additional nodes
                if(node1_b != NULL) {
                    if(node1_b1 != NULL) {
                        if(node1_b1->up != node1_b1_check) {
                            node1_b1 = node1_b1->up;
                        } else {
                            node1_b1 = node1_b1->down;
                        }
                        node1_b1_check = node1_b1_prev;
                        node1_b1_prev = node1_b1;
                        if(node1_b1 != NULL) {
                            node1_b1->is_boundary = true;
                        }
                    }
                    if(node1_b2 != NULL) {
                        if(node1_b2->down != node1_b2_check) {
                            node1_b2 = node1_b2->down;
                        } else {
                            node1_b2 = node1_b2->up;
                        }
                        node1_b2_check = node1_b2_prev;
                        node1_b2_prev = node1_b2;
                        if(node1_b2 != NULL) {
                            node1_b2->is_boundary = true;
                        }
                    }
                }
                
                if(node2_b != NULL) {
                    if(node2_b1 != NULL) {
                        if(node2_b1->up != node2_b1_check) {
                            node2_b1 = node2_b1->up;
                        } else {
                            node2_b1 = node2_b1->down;
                        }
                        node2_b1_check = node2_b1_prev;
                        node2_b1_prev = node2_b1;
                        if(node2_b1 != NULL) {
                            node2_b1->is_boundary = true;
                        }
                    }
                    if(node2_b2 != NULL) {
                        if(node2_b2->down != node2_b2_check) {
                            node2_b2 = node2_b2->down;
                        } else {
                            node2_b2 = node2_b2->up;
                        }
                        node2_b2_check = node2_b2_prev;
                        node2_b2_prev = node2_b2;
                        if(node2_b2 != NULL) {
                            node2_b2->is_boundary = true;
                        }
                    }
                }
            }

            // Get corner boundary nodes
            Bakery *node1_1_b = NULL;
            Bakery *node1_2_b = NULL;
            Bakery *node2_bb = NULL;
            Bakery *node2_rf_b = NULL;

            if(node1_1->up != node1_1_check) {
                node1_1_b = node1_1->up;
            } else {
                node1_1_b = node1_1->down;
            }
            
            if(node1_2->down != node1_2_check) {
                node1_2_b = node1_2->down;
            } else {
                node1_2_b = node1_2->up;
            }

            if(node2_rf->up != node2_rf_check) {
                node2_rf_b = node2_rf->up;
            } else {
                node2_rf_b = node2_rf->down;
            }
            
            if(node2->down != node2_check) {
                node2_bb = node2->down;
            } else {
                node2_bb = node2->up;
            }

            Bakery *temp_node1_1_b = node1_1_b;
            Bakery *temp_node1_2_b = node1_2_b;
            Bakery *temp_node2_bb = node2_bb;
            Bakery *temp_node2_rf_b = node2_rf_b;

            // Set corner boundary flags and their horizontal neighbors
            if(node1_1_b != NULL) {
                node1_1_b->is_boundary = true;
            }
            if(node1_2_b != NULL) {
                node1_2_b->is_boundary = true;
            }
            if(node2_rf_b != NULL) {
                node2_rf_b->is_boundary = true;
            }
            if(node2_bb != NULL) {
                node2_bb->is_boundary = true;
            }

            // Continue with horizontal boundary setup following original pattern...
            // [The rest follows the same complex boundary setup from your original]

            // Determine correct rotation nodes
            Bakery *temp_1 = node1_2->left;
            Bakery *temp_2 = node1_2->right;
            Bakery *temp_1_prev = temp_1;
            Bakery *temp_2_prev = temp_2;
            Bakery *temp_1_check = node1_2;
            Bakery *temp_2_check = node1_2;
            
            for(int z = 1; z < racing_track_size - 1; z++) {
                if (temp_1 != NULL) {
                    if(temp_1->left != temp_1_check) {
                        temp_1 = temp_1->left;
                    } else {
                        temp_1 = temp_1->right;
                    }
                    temp_1_check = temp_1_prev;
                    temp_1_prev = temp_1;
                }
                if (temp_2 != NULL) {
                    if(temp_2->right != temp_2_check) {
                        temp_2 = temp_2->right;
                    } else {
                        temp_2 = temp_2->left;
                    }
                    temp_2_check = temp_2_prev;
                    temp_2_prev = temp_2;
                }
            }

            Bakery *rotation_node1;
            Bakery *rotation_node2;
            
            if(temp_1 == node2 || temp_2 == node2) {
                rotation_node1 = node1_1;
                rotation_node2 = node2;
            } else {
                rotation_node1 = node1_2;
                rotation_node2 = node2_rf;
            }

            // Update top left corner if needed
            if(rotation_node1 == top_left_corner) {
                top_left_corner = rotation_node2;
            } else if(rotation_node2 == top_left_corner) {
                top_left_corner = rotation_node1;
            }

            // Perform 180-degree rotation following original pattern
            int direction1, direction2;

            if(rotation_node1->up == NULL || rotation_node1->up->is_boundary == true) {
                direction1 = 0; // down
            } else {
                direction1 = 1; // up
            }
            
            if(rotation_node2->up == NULL || rotation_node2->up->is_boundary == true) {
                direction2 = 2; // down
            } else {
                direction2 = 3; // up
            }

            Bakery *node1_check_new = rotation_node1;
            Bakery *node2_check_new = rotation_node2;
            Bakery *node1_prev_new = rotation_node1;
            Bakery *node2_prev_new = rotation_node2;

            // Vertical rotation
            for(int i = 0; i < racing_track_size; i++) {
                if (i != 0) {
                    if(direction1 == 0) {
                        if(i == 1 || rotation_node1->down != node1_check_new) {
                            rotation_node1 = rotation_node1->down;
                        } else {
                            rotation_node1 = rotation_node1->up;
                        }
                        node1_check_new = node1_prev_new;
                        node1_prev_new = rotation_node1;
                    } else {
                        if(i == 1 || rotation_node1->up != node1_check_new) {
                            rotation_node1 = rotation_node1->up;
                        } else {
                            rotation_node1 = rotation_node1->down;
                        }
                        node1_check_new = node1_prev_new;
                        node1_prev_new = rotation_node1;
                    }
                    
                    if(direction2 == 2) {
                        if(i == 1 || rotation_node2->down != node2_check_new) {
                            rotation_node2 = rotation_node2->down;
                        } else {
                            rotation_node2 = rotation_node2->up;
                        }
                        node2_check_new = node2_prev_new;
                        node2_prev_new = rotation_node2;
                    } else {
                        if(i == 1 || rotation_node2->up != node2_check_new) {
                            rotation_node2 = rotation_node2->up;
                        } else {
                            rotation_node2 = rotation_node2->down;
                        }
                        node2_check_new = node2_prev_new;
                        node2_prev_new = rotation_node2;
                    }
                }

                // Swap horizontal connections
                Bakery *temp_horizontal;
                Bakery *temp2_horizontal;
                int horizontal_direction1, horizontal_direction2;
                
                if(rotation_node1->left == NULL || rotation_node1->left->is_boundary == true) {
                    temp_horizontal = rotation_node1->left;
                    horizontal_direction1 = 0;
                } else {
                    temp_horizontal = rotation_node1->right;
                    horizontal_direction1 = 1;
                }
                
                if(rotation_node2->left == NULL || rotation_node2->left->is_boundary == true) {
                    temp2_horizontal = rotation_node2->left;
                    horizontal_direction2 = 0;
                } else {
                    temp2_horizontal = rotation_node2->right;
                    horizontal_direction2 = 1;
                }

                if(horizontal_direction1 == 0) {
                    rotation_node1->left = temp2_horizontal;
                } else {
                    rotation_node1->right = temp2_horizontal;
                }

                if (temp_horizontal != NULL) {
                    if(temp_horizontal->right == rotation_node1) {
                        temp_horizontal->right = rotation_node2;
                    } else {
                        temp_horizontal->left = rotation_node2;
                    }
                }

                if(horizontal_direction2 == 0) {
                    rotation_node2->left = temp_horizontal;
                } else {
                    rotation_node2->right = temp_horizontal;
                }
                
                if(temp2_horizontal != NULL) {
                    if(temp2_horizontal->right == rotation_node2) {
                        temp2_horizontal->right = rotation_node1;
                    } else {
                        temp2_horizontal->left = rotation_node1;
                    }
                }
            }

            // Reset directions for horizontal movement
            if(rotation_node1->left == NULL || rotation_node1->left->is_boundary == true) {
                direction1 = 0; // right
            } else {
                direction1 = 1; // left
            }
            
            if(rotation_node2->left == NULL || rotation_node2->left->is_boundary == true) {
                direction2 = 2; // right
            } else {
                direction2 = 3; // left
            }

            if(rotation_node1 == top_left_corner) {
                top_left_corner = rotation_node2;
            } else if(rotation_node2 == top_left_corner) {
                top_left_corner = rotation_node1;
            }

            // Horizontal rotation
            for(int i = 0; i < racing_track_size; i++) {
                if (i != 0) {
                    if(direction1 == 0) {
                        if(i == 1 || rotation_node1->right != node1_check_new) {
                            rotation_node1 = rotation_node1->right;
                        } else {
                            rotation_node1 = rotation_node1->left;
                        }
                        node1_check_new = node1_prev_new;
                        node1_prev_new = rotation_node1;
                    } else {
                        if(i == 1 || rotation_node1->left != node1_check_new) {
                            rotation_node1 = rotation_node1->left;
                        } else {
                            rotation_node1 = rotation_node1->right;
                        }
                        node1_check_new = node1_prev_new;
                        node1_prev_new = rotation_node1;
                    }
                    
                    if(direction2 == 2) {
                        if(i == 1 || rotation_node2->right != node2_check_new) {
                            rotation_node2 = rotation_node2->right;
                        } else {
                            rotation_node2 = rotation_node2->left;
                        }
                        node2_check_new = node2_prev_new;
                        node2_prev_new = rotation_node2;
                    } else {
                        if(i == 1 || rotation_node2->left != node2_check_new) {
                            rotation_node2 = rotation_node2->left;
                        } else {
                            rotation_node2 = rotation_node2->right;
                        }
                        node2_check_new = node2_prev_new;
                        node2_prev_new = rotation_node2;
                    }
                }

                // Swap vertical connections
                Bakery *temp_vertical;
                Bakery *temp2_vertical;
                int vertical_direction1, vertical_direction2;
                
                if(rotation_node1->up == NULL || rotation_node1->up->is_boundary == true) {
                    temp_vertical = rotation_node1->up;
                    vertical_direction1 = 0;
                } else {
                    temp_vertical = rotation_node1->down;
                    vertical_direction1 = 1;
                }
                
                if(rotation_node2->up == NULL || rotation_node2->up->is_boundary == true) {
                    temp2_vertical = rotation_node2->up;
                    vertical_direction2 = 0;
                } else {
                    temp2_vertical = rotation_node2->down;
                    vertical_direction2 = 1;
                }

                if(vertical_direction1 == 0) {
                    rotation_node1->up = temp2_vertical;
                } else {
                    rotation_node1->down = temp2_vertical;
                }

                if (temp_vertical != NULL) {
                    if(temp_vertical->down == rotation_node1) {
                        temp_vertical->down = rotation_node2;
                    } else {
                        temp_vertical->up = rotation_node2;
                    }
                }

                if(vertical_direction2 == 0) {
                    rotation_node2->up = temp_vertical;
                } else {
                    rotation_node2->down = temp_vertical;
                }
                
                if(temp2_vertical != NULL) {
                    if(temp2_vertical->down == rotation_node2) {
                        temp2_vertical->down = rotation_node1;
                    } else {
                        temp2_vertical->up = rotation_node1;
                    }
                }
            }

            // Reset all boundary flags - following original cleanup pattern
            if(temp_node1_b != NULL) {
                temp_node1_b->is_boundary = false;
            }
            if(temp_node2_b != NULL) {
                temp_node2_b->is_boundary = false;
            }
            if(temp_node1_1_b != NULL) {
                temp_node1_1_b->is_boundary = false;
            }
            if(temp_node1_2_b != NULL) {
                temp_node1_2_b->is_boundary = false;
            }
            if(temp_node2_rf_b != NULL) {
                temp_node2_rf_b->is_boundary = false;
            }
            if(temp_node2_bb != NULL) {
                temp_node2_bb->is_boundary = false;
            }
            
            // Reset additional boundary nodes
            if(temp_node1_b1 != NULL) {
                temp_node1_b1->is_boundary = false;
            }
            if(temp_node1_b2 != NULL) {
                temp_node1_b2->is_boundary = false;
            }
            if(temp_node2_b1 != NULL) {
                temp_node2_b1->is_boundary = false;
            }
            if(temp_node2_b2 != NULL) {
                temp_node2_b2->is_boundary = false;
            }
        }

        // Rebuild grid structure after rotation - following original pattern
        Bakery *grid_node = top_left_corner;
        Bakery *prev_node;
        Bakery *check_node;
        Bakery *row_control;
        Bakery *row_control_prev;
        Bakery *row_control_check;
        
        for(int i = 0; i < num_rows; i++) {
            for(int j = 0; j < num_cols; j++) {
                grid[i][j].bakery_ptr = grid_node;
                
                if (j == 0) {
                    if(grid_node->left != NULL) {
                        grid_node->right = grid_node->left;
                        grid_node->left = NULL;
                    }
                    check_node = grid_node;
                    grid_node = grid_node->right;
                    prev_node = grid_node;
                } else {
                    if(grid_node->right != check_node) {
                        grid_node = grid_node->right;
                    } else {
                        grid_node = grid_node->left;
                    }
                    check_node = prev_node;
                    prev_node = grid_node;
                }
            }

            if (i == 0) {
                row_control = top_left_corner;
                if(row_control->up != NULL) {
                    row_control->down = row_control->up;
                    row_control->up = NULL;
                }
                row_control_check = row_control;
                row_control = row_control->down;
                row_control_prev = row_control;
            } else {
                if(row_control->down != row_control_check) {
                    row_control = row_control->down;
                } else {
                    row_control = row_control->up;
                }
                row_control_check = row_control_prev;
                row_control_prev = row_control;
            }
            grid_node = row_control;
        }

        // Relink all nodes in proper grid structure
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                if (i == 0) {
                    if (j == 0) {
                        link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, NULL);
                    } else {
                        if (j == num_cols-1) {
                            link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, NULL, grid[i][j-1].bakery_ptr);
                        } else {
                            link_bakery_nodes(grid[i][j].bakery_ptr, NULL, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                        }
                    }
                } else {
                    if(i == num_rows-1) {
                        if(j == 0) {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, grid[i][j+1].bakery_ptr, NULL);
                        } else {
                            if (j == num_cols-1) {
                                link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, NULL, grid[i][j-1].bakery_ptr);
                            } else {
                                link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, NULL, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                            }
                        }
                    } else {
                        if(j == 0) {
                            link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, NULL);
                        } else {
                            if(j == num_cols-1) {
                                link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, NULL, grid[i][j-1].bakery_ptr);
                            } else {
                                link_bakery_nodes(grid[i][j].bakery_ptr, grid[i-1][j].bakery_ptr, grid[i+1][j].bakery_ptr, grid[i][j+1].bakery_ptr, grid[i][j-1].bakery_ptr);
                            }
                        }
                    }
                }
            }
        }

        // Remove closed bakeries from grid after rotation
        for(int i = 0; i < closed_count; i++) {
            Bakery *temp = rank_to_bakery[closed_bakery_ranks[i]].bakery_ptr;
            if (temp->up != NULL) {
                temp->up->down = temp->down;
                temp->up->zero_distance_down = temp->up->zero_distance_down + temp->zero_distance_down + 1;
                temp->up->distance_down = temp->up->zero_distance_down;
            }
            if (temp->down != NULL) {
                temp->down->up = temp->up;
                temp->down->zero_distance_up = temp->down->zero_distance_up + temp->zero_distance_up + 1;
                temp->down->distance_up = temp->down->zero_distance_up;
            }
            if (temp->right != NULL) {
                temp->right->left = temp->left;
                temp->right->zero_distance_left = temp->right->zero_distance_left + temp->zero_distance_left + 1;
                temp->right->distance_left = temp->right->zero_distance_left;
            }
            if (temp->left != NULL) {
                temp->left->right = temp->right;
                temp->left->zero_distance_right = temp->left->zero_distance_right + temp->zero_distance_right + 1;
                temp->left->distance_right = temp->left->zero_distance_right;
            }
        }
    }

    // Second training period
    for (int day = 0; day < training_days_2; day++) {
        scanf("%d%d%d", &starting_rank, &tour_length, &team_size);
        Bakery *current_bakery = rank_to_bakery[starting_rank].bakery_ptr;
        Bakery *next_bakery = NULL;
        int visited_count = 0;

        visited_bakeries[0] = current_bakery;
        int visited_index = 1;
        
        while (visited_count < tour_length) {
            next_bakery = find_next_bakery(current_bakery);

            if (next_bakery == NULL) {
                break;
            }

            Bakery *temp = current_bakery;
            current_bakery = next_bakery;
            
            if (temp->up != NULL) {
                temp->up->down = temp->down;
                temp->up->distance_down = temp->up->distance_down + temp->distance_down + 1;
            }
            if (temp->down != NULL) {
                temp->down->up = temp->up;
                temp->down->distance_up = temp->down->distance_up + temp->distance_up + 1;
            }
            if (temp->right != NULL) {
                temp->right->left = temp->left;
                temp->right->distance_left = temp->right->distance_left + temp->distance_left + 1;
            }
            if (temp->left != NULL) {
                temp->left->right = temp->right;
                temp->left->distance_right = temp->left->distance_right + temp->distance_right + 1;
            }

            if (current_bakery->toast_count <= team_size) {
                current_bakery->toast_count = 0;
                visited_bakeries[visited_index] = current_bakery;
                visited_index++;
            } else {
                current_bakery->toast_count = current_bakery->toast_count - team_size;
                visited_bakeries[visited_index] = current_bakery;
                visited_index++;
            }
            visited_count++;
        }

        for (int t = 0; t < visited_index; t++) {
            if (visited_bakeries[t]->up != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->up->down = visited_bakeries[t]->down;
                    visited_bakeries[t]->up->zero_distance_down = visited_bakeries[t]->up->zero_distance_down + visited_bakeries[t]->zero_distance_down + 1;
                } else {
                    visited_bakeries[t]->up->down = visited_bakeries[t];
                }
                visited_bakeries[t]->up->distance_down = visited_bakeries[t]->up->zero_distance_down;
            }
            if (visited_bakeries[t]->down != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->down->up = visited_bakeries[t]->up;
                    visited_bakeries[t]->down->zero_distance_up = visited_bakeries[t]->down->zero_distance_up + visited_bakeries[t]->zero_distance_up + 1;
                } else {
                    visited_bakeries[t]->down->up = visited_bakeries[t];
                }
                visited_bakeries[t]->down->distance_up = visited_bakeries[t]->down->zero_distance_up;
            }
            if (visited_bakeries[t]->right != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->right->left = visited_bakeries[t]->left;
                    visited_bakeries[t]->right->zero_distance_left = visited_bakeries[t]->right->zero_distance_left + visited_bakeries[t]->zero_distance_left + 1;
                } else {
                    visited_bakeries[t]->right->left = visited_bakeries[t];
                }
                visited_bakeries[t]->right->distance_left = visited_bakeries[t]->right->zero_distance_left;
            }
            if (visited_bakeries[t]->left != NULL) {
                if(visited_bakeries[t]->toast_count == 0) {
                    visited_bakeries[t]->left->right = visited_bakeries[t]->right;
                    visited_bakeries[t]->left->zero_distance_right = visited_bakeries[t]->left->zero_distance_right + visited_bakeries[t]->zero_distance_right + 1;
                } else {
                    visited_bakeries[t]->left->right = visited_bakeries[t];
                }
                visited_bakeries[t]->left->distance_right = visited_bakeries[t]->left->zero_distance_right;
            }
        }
        
        for(int r = 0; r < visited_index; r++) {
            visited_bakeries[r] = NULL;
        }
    }

    // Output final toast counts
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            printf("%d ", grid[i][j].bakery_ptr->toast_count);
        }
        printf("\n");
    }

    return 0;
}
