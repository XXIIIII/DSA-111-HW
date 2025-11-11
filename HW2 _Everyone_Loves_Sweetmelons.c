#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#define MAX_COMPANIES 1000001

typedef struct sales_event_node
{
    long long melon_price;
    int expiry_day;
} SalesEvent;

typedef struct company_node
{
    int company_id;
    int *children_ids;
    int children_count;
    int total_subtree_companies;
    long long total_subtree_cost;
    int active_events_count;
    SalesEvent **min_heap_events;
    struct company_node *first_child;
    struct company_node *next_sibling;
} Company;

Company *create_company_node(int id, Company *first_child, Company *next_sibling)
{
    Company *company = (Company *)malloc(sizeof(Company));
    assert(company != NULL);
    company->company_id = id;
    company->children_ids = NULL;
    company->children_count = 0;
    company->total_subtree_cost = 0;
    company->active_events_count = 0;
    company->min_heap_events = NULL;
    company->first_child = first_child;
    company->next_sibling = next_sibling;
    return company;
}

SalesEvent *create_sales_event(long long price, int expiry_day)
{
    SalesEvent *event = (SalesEvent *)malloc(sizeof(SalesEvent));
    assert(event != NULL);
    event->melon_price = price;
    event->expiry_day = expiry_day;
    return event;
}

int get_next_company_id(void)
{
    static int id_counter = 0;
    id_counter++;
    return id_counter;
}

void maintain_min_heap_upward(SalesEvent **heap_array, int current_index, int day)
{
    int parent_index = (current_index - 1) / 2;
    if (parent_index >= 0 && parent_index != current_index)
    {
        if (heap_array[parent_index]->melon_price > heap_array[current_index]->melon_price)
        {
            SalesEvent *temp = heap_array[parent_index];
            heap_array[parent_index] = heap_array[current_index];
            heap_array[current_index] = temp;
            maintain_min_heap_upward(heap_array, parent_index, day);
        }
    }
}

void insert_sales_event_to_heap(SalesEvent *new_event, SalesEvent **heap_array, int heap_size, int current_day)
{
    int insertion_index = heap_size;
    if (new_event->expiry_day <= heap_array[0]->expiry_day && new_event->melon_price >= heap_array[0]->melon_price)
    {
        new_event->melon_price = 1000000001;
    }
    heap_array[insertion_index] = new_event;
    maintain_min_heap_upward(heap_array, insertion_index, current_day);
}

void maintain_min_heap_downward(SalesEvent **heap_array, int heap_size, int current_index)
{
    int smallest_index = current_index;
    int left_child_index, right_child_index;
    SalesEvent *temp = NULL;
    
    while(true)
    {
        left_child_index = 2 * current_index + 1;
        right_child_index = 2 * current_index + 2;
        
        if(left_child_index > heap_size)
            break;
            
        if(left_child_index < heap_size && heap_array[left_child_index]->melon_price < heap_array[current_index]->melon_price)
        {
            smallest_index = left_child_index;
        }
        
        if(right_child_index < heap_size && heap_array[right_child_index]->melon_price < heap_array[smallest_index]->melon_price)
        {
            smallest_index = right_child_index;
        }
        
        if(smallest_index == current_index)
            break;
        else
        {
            temp = heap_array[current_index];
            heap_array[current_index] = heap_array[smallest_index];
            heap_array[smallest_index] = temp;
            current_index = smallest_index;
        }
    }
}

void calculate_subtree_costs(Company **company_array, int company_id, long long daily_budget, int *parent_lookup)
{
    int parent_id;
    
    if(company_array[company_id]->children_count == 0)
    {
        parent_id = parent_lookup[company_id];
        if(parent_id != 0)
        {
            company_array[parent_id]->total_subtree_cost += company_array[company_id]->total_subtree_cost;
            company_array[parent_id]->total_subtree_companies += company_array[company_id]->total_subtree_companies;
        }
    }
    else
    {
        int child_index = 0;
        while(child_index < company_array[company_id]->children_count)
        {
            calculate_subtree_costs(company_array, company_array[company_id]->children_ids[child_index], daily_budget, parent_lookup);
            child_index++;
            
            if(child_index >= company_array[company_id]->children_count)
            {
                parent_id = parent_lookup[company_id];
                if(parent_id != 0)
                {
                    company_array[parent_id]->total_subtree_cost += company_array[company_id]->total_subtree_cost;
                    company_array[parent_id]->total_subtree_companies += company_array[company_id]->total_subtree_companies;
                }
            }
        }
    }
}

signed main()
{
    int daily_results[MAX_COMPANIES] = {0};
    int total_companies, total_days;
    long long daily_budget;
    int current_company_id = get_next_company_id();
    Company *company_array[MAX_COMPANIES];
    int children_count_per_company[MAX_COMPANIES] = {0};
    int parent_lookup[MAX_COMPANIES] = {0};
    
    scanf("%d%d%lld", &total_companies, &total_days, &daily_budget);
    
    Company *root_company = create_company_node(current_company_id, NULL, NULL);
    company_array[1] = root_company;
    int parent_company_id;
    
    // Create company tree structure
    for (int i = 1; i < total_companies; i++)
    {
        current_company_id = get_next_company_id();
        Company *new_company = create_company_node(current_company_id, NULL, NULL);
        company_array[current_company_id] = new_company;
        scanf("%d", &parent_company_id);
        children_count_per_company[parent_company_id]++;
        parent_lookup[current_company_id] = parent_company_id;
    }
    
    for (int i = 1; i <= total_companies; i++)
    {
        if(children_count_per_company[i] != 0)
        {
            company_array[i]->children_ids = (int *)malloc(children_count_per_company[i] * sizeof(int));
        }
    }
    
    for (int i = 2; i <= total_companies; i++)
    {
        company_array[parent_lookup[i]]->children_ids[company_array[parent_lookup[i]]->children_count] = i;
        company_array[parent_lookup[i]]->children_count += 1;
    }

    long long melon_price;
    int sale_duration;
    
    for(int current_day = 1; current_day <= total_days; current_day++)
    {
        for(int company_id = 1; company_id <= total_companies; company_id++)
        {
            scanf("%lld", &melon_price);
            scanf("%d", &sale_duration);

            SalesEvent *new_sales_event = create_sales_event(melon_price, sale_duration + current_day);
            SalesEvent **heap_pointer = company_array[company_id]->min_heap_events;
            
            // Build/maintain min heap for sales events
            if(company_array[company_id]->active_events_count == 0)
            {
                heap_pointer = (SalesEvent **)malloc(total_days * sizeof(SalesEvent*));
                company_array[company_id]->min_heap_events = heap_pointer;
                heap_pointer[0] = new_sales_event;
            }
            else
            {
                insert_sales_event_to_heap(new_sales_event, heap_pointer, company_array[company_id]->active_events_count, current_day);
            }
            
            // Remove expired events from heap root
            SalesEvent *expired_event;
            while(heap_pointer[0]->expiry_day < current_day)
            {
                expired_event = heap_pointer[0];
                heap_pointer[0] = heap_pointer[company_array[company_id]->active_events_count];
                heap_pointer[company_array[company_id]->active_events_count] = expired_event;
                heap_pointer[company_array[company_id]->active_events_count]->melon_price = 1000000001;
                maintain_min_heap_downward(heap_pointer, company_array[company_id]->active_events_count, 0);
            }
            company_array[company_id]->active_events_count += 1;
        }
        
        for(int company_id = 1; company_id <= total_companies; company_id++)
        {
            company_array[company_id]->total_subtree_cost = company_array[company_id]->min_heap_events[0]->melon_price;
            company_array[company_id]->total_subtree_companies = company_array[company_id]->children_count;
        }
        
        calculate_subtree_costs(company_array, 1, daily_budget, parent_lookup);
        int max_melons_today = 0;

        for(int company_id = 1; company_id <= total_companies; company_id++)
        {
            if(company_array[company_id]->total_subtree_cost <= daily_budget)
            {
                if(1 + company_array[company_id]->total_subtree_companies > max_melons_today)
                {
                    max_melons_today = 1 + company_array[company_id]->total_subtree_companies;
                }
            }
        }
        daily_results[current_day] = max_melons_today;
    }

    for(int day = 1; day <= total_days; day++)
    {
        printf("%d\n", daily_results[day]);
    }

    return 0;
}
