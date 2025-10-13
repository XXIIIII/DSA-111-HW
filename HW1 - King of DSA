#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

typedef struct player_node
{
    unsigned int player_id;
    unsigned int attack_power;
    struct player_node *next;
    struct player_node *prev;
} Player;

Player *manage_arena_tail(Player *node, char operation)
{
    static Player *arena_tail = NULL;
    if (operation == '1')
    {
        arena_tail = node;
        return NULL;
    }
    else
    {
        return arena_tail;
    }
}

unsigned int manage_arena_count(char operation)
{
    static unsigned int player_count = 0;
    if (operation == '1')
    {
        player_count++;
        return 0;
    }
    else if(operation == '2')
    {
        player_count--;
        return 0;
    }
    else if(operation == '3')
    {
        return player_count;
    }
}

void print_round_header(void)
{
    static int round_number = 0;
    round_number++;
    printf("Round %d: ", round_number);
}

Player *create_player_node(unsigned int id, unsigned int power, Player* next, Player* prev)
{
    Player *node = (Player *)malloc(sizeof(Player));
    assert(node != NULL);
    node->player_id = id;
    node->attack_power = power;
    node->next = next;
    node->prev = prev;
    return node;
}

Player *insert_player_by_power(Player *head, Player *new_player, unsigned int count, unsigned int power)
{
    Player *current = head;
    Player *previous = NULL;
    Player *dummy = NULL;
    
    if (head == NULL)
    {
        dummy = manage_arena_tail(new_player, '1');
        return (new_player);
    }
    
    while(current != NULL && power > current->attack_power)
    {
        previous = current;
        current = current->next;
    }
    
    if (previous == NULL)  // new player has lowest power
    {
        new_player->next = head;
        head->prev = new_player;
        return (new_player);
    }
    else   // insert in middle or end
    {
        if (current != NULL)
        {
            previous->next->prev = new_player;
        }

        new_player->next = previous->next;
        previous->next = new_player;
        new_player->prev = previous;
        
        if (current == NULL)
        {
            dummy = manage_arena_tail(new_player, '1'); // new player has highest power
        }

        return (head);
    }
}

void print_killed_players(Player *head, Player *survivor)
{
    for(; head != survivor; head = head->next)
    {
        printf("%u ", head->player_id);
    }
}

void remove_killed_players(Player *head, Player *survivor)
{
    int temp_var;
    while(head != survivor)
    {
        Player *next_player = head->next;
        free(head);
        temp_var = manage_arena_count('2');
        head = next_player;
    }
}

int main()
{
    unsigned int total_players, max_arena_size, current_attack_power, current_player_id = 1;
    Player *arena_head = NULL;
    Player *arena_tail = NULL;
    int temp_increment = 0;

    scanf("%u%u", &total_players, &max_arena_size);
    
    while(current_player_id != total_players + 1)
    {
        scanf("%u", &current_attack_power);
        print_round_header();
        
        Player *new_player = create_player_node(current_player_id, current_attack_power, NULL, NULL);
        temp_increment = manage_arena_count('1');
        arena_head = insert_player_by_power(arena_head, new_player, current_player_id, current_attack_power);
        
        print_killed_players(arena_head, new_player);
        remove_killed_players(arena_head, new_player);
        
        arena_head = new_player;
        new_player->prev = NULL;
        
        int current_arena_size = manage_arena_count('3');
        if (current_arena_size > max_arena_size)
        {
            arena_tail = manage_arena_tail(NULL, '2');
            printf("%u", arena_tail->player_id);
            arena_tail->prev->next = NULL;
            Player *dummy = manage_arena_tail(arena_tail->prev, '1');
            free(arena_tail);
            temp_increment = manage_arena_count('2');
        }

        current_player_id++;
        printf("\n");
    }
    
    printf("Final: ");
    print_killed_players(arena_head, NULL);

    return 0;
}
