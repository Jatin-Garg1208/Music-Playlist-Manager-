#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct playlist
{
    char playlist_title[20];
    char playlist_artist[20];
    int duration;
    struct playlist *next;
    struct playlist *prev;
};

struct action
{
    char action_type[10];
    struct playlist *song_detail;
    struct action *next;
};

struct stack
{
    struct action *top;
};

struct stack *undostack = NULL;
struct stack *redostack = NULL;
struct playlist *start = NULL;

struct playlist *create(struct playlist *start);
struct playlist *deleted(struct playlist *start);
void display(struct playlist *start);
void push(struct stack *stack, char *action_type, struct playlist *song_detail);
struct action *pop(struct stack *stack);
void undo(struct stack *undostack, struct stack *redostack, struct playlist **start);
void redo(struct stack *redostack, struct stack *undostack, struct playlist **start);

int main()
{
    undostack = (struct stack *)malloc(sizeof(struct stack));
    redostack = (struct stack *)malloc(sizeof(struct stack));
    undostack->top = NULL;
    redostack->top = NULL;

    int choice;
    do
    {
        printf("-----Music Playlist-----\n");
        printf("Press Number:- \n1) Create/Insert \n2) Delete \n3) Undo \n4) Redo \n5) Display \n6) Exit\n");
        printf("------------------------\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            start = create(start);
            break;
        case 2:
            start = deleted(start);
            break;
        case 3:
            undo(undostack, redostack, &start);
            break;
        case 4:
            redo(redostack, undostack, &start);
            break;
        case 5:
            display(start);
            break;
        case 6:
            printf("\n-----Exiting-----\n");
            break;
        default:
            printf("\nInvalid Choice Enter\n");
        }
    } while (choice != 6);

    // Free memory for stacks
    struct action *temp;
    while (undostack->top != NULL)
    {
        temp = undostack->top;
        undostack->top = undostack->top->next;
        free(temp);
    }
    free(undostack);
    while (redostack->top != NULL)
    {
        temp = redostack->top;
        redostack->top = redostack->top->next;
        free(temp);
    }
    free(redostack);

    // Free memory for playlist
    struct playlist *current = start;
    if (current != NULL)
    {
        do
        {
            struct playlist *temp = current;
            current = current->next;
            free(temp);
        } while (current != start);
    }

    return 0;
}

struct playlist *create(struct playlist *start)
{
    int time = 0;
    printf("\nEnter -1 to Exit\n");
    while (1)
    {
        printf("\nEnter the duration of the Song in seconds: ");
        scanf("%d", &time);
        getchar();
        if (time == -1)
        {
            break;
        }
        char artist_name[20];
        char playlist_name[20];
        printf("Enter the name of the artist: ");
        fgets(artist_name, 20, stdin);
        artist_name[strcspn(artist_name, "\n")] = 0;
        printf("Enter the name of the song: ");
        fgets(playlist_name, 20, stdin);
        playlist_name[strcspn(playlist_name, "\n")] = 0;
        struct playlist *new_playlist, *ptr;
        new_playlist = (struct playlist *)malloc(sizeof(struct playlist));
        if (!new_playlist)
        {
            printf("\nMemory overflow \n");
            return start;
        }
        new_playlist->duration = time;
        strcpy(new_playlist->playlist_artist, artist_name);
        strcpy(new_playlist->playlist_title, playlist_name);
        new_playlist->next = NULL;
        new_playlist->prev = NULL;
        if (start == NULL)
        {
            start = new_playlist;
            new_playlist->next = start;
            new_playlist->prev = start;
        }
        else
        {
            ptr = start;
            while (ptr->next != start)
            {
                ptr = ptr->next;
            }
            ptr->next = new_playlist;
            new_playlist->prev = ptr;
            new_playlist->next = start;
            start->prev = new_playlist;
        }
        push(undostack, "add", new_playlist);
        display(start);
    }
    return start;
}

struct playlist *deleted(struct playlist *start)
{
    if (start == NULL)
    {
        printf("\n Playlist is empty");
        return start;
    }
    else
    {
        struct playlist *ptr;
        ptr = start;
        char playlist_name[20];
        printf("\nEnter playlist title: ");
        fgets(playlist_name, 20, stdin);
        playlist_name[strcspn(playlist_name, "\n")] = 0;
        do
        {
            if (strcmp(ptr->playlist_title, playlist_name) == 0)
            {
                if (ptr->next == ptr)
                {
                    start = NULL;
                }
                else
                {
                    ptr->prev->next = ptr->next;
                    ptr->next->prev = ptr->prev;
                    if (ptr == start)
                    {
                        start = ptr->next;
                    }
                }
                push(undostack, "delete", ptr);
                display(start);
                return start;
            }
            ptr = ptr->next;
        } while (ptr != start);
        printf("Playlist not found.\n");
        return start;
    }
}

void display(struct playlist *start)
{
    if (start == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    struct playlist *ptr = start;
    printf("Data: \n");
    do
    {
        printf("Song Title: %s, Song Artist: %s, Duration: %d\n", ptr->playlist_title, ptr->playlist_artist, ptr->duration);
        ptr = ptr->next;
    } while (ptr != start);
}

void push(struct stack *stack, char *action_type, struct playlist *song_detail)
{
    struct action *Action;
    Action = (struct action *)malloc(sizeof(struct action));
    if (!Action)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    strcpy(Action->action_type, action_type);
    Action->song_detail = song_detail; // Store the pointer directly
    Action->next = stack->top;
    stack->top = Action;
}

struct action *pop(struct stack *stack)
{
    if (stack->top == NULL)
    {
        printf("Stack is empty.\n");
        return NULL;
    }
    struct action *Action = stack->top;
    stack->top = stack->top->next;
    return Action;
}

void undo(struct stack *undostack, struct stack *redostack, struct playlist **start)
{
    struct action *Action = pop(undostack);
    if (Action == NULL)
    {
        printf("Nothing to undo.\n");
        return;
    }
    if (strcmp(Action->action_type, "add") == 0)
    {
        struct playlist *ptr = *start;
        if (ptr == Action->song_detail)
        {
            *start = (*start)->next;
        }
        if (Action->song_detail->prev != NULL)
        {
            Action->song_detail->prev->next = Action->song_detail->next;
        }
        if (Action->song_detail->next != NULL)
        {
            Action->song_detail->next->prev = Action->song_detail->prev;
        }
        push(redostack, "add", Action->song_detail);
    }
    else if (strcmp(Action->action_type, "delete") == 0)
    {
        struct playlist *ptr = *start;
        if (*start == NULL)
        {
            *start = Action->song_detail;
            Action->song_detail->next = *start;
            Action->song_detail->prev = *start;
        }
        else
        {
            while (ptr->next != *start)
            {
                ptr = ptr->next;
            }
            ptr->next = Action->song_detail;
            Action->song_detail->prev = ptr;
            Action->song_detail->next = *start;
            (*start)->prev = Action->song_detail;
        }
        push(redostack, "delete", Action->song_detail);
    }
    free(Action);
}

void redo(struct stack *redostack, struct stack *undostack, struct playlist **start)
{
    struct action *Action = pop(redostack);
    if (Action == NULL)
    {
        printf("Nothing to redo.\n");
        return;
    }
    if (strcmp(Action->action_type, "delete") == 0)
    {
        struct playlist *ptr = *start;
        if (ptr == Action->song_detail)
        {
            *start = (*start)->next;
        }
        if (Action->song_detail->prev != NULL)
        {
            Action->song_detail->prev->next = Action->song_detail->next;
        }
        if (Action->song_detail->next != NULL)
        {
            Action->song_detail->next->prev = Action->song_detail->prev;
        }
        push(undostack, "delete", Action->song_detail);
    }
    else if (strcmp(Action->action_type, "add") == 0)
    {
        struct playlist *ptr = *start;
        if (*start == NULL)
        {
            *start = Action->song_detail;
            Action->song_detail->next = *start;
            Action->song_detail->prev = *start;
        }
        else
        {
            while (ptr->next != *start)
            {
                ptr = ptr->next;
            }
            ptr->next = Action->song_detail;
            Action->song_detail->prev = ptr;
            Action->song_detail->next = *start;
            (*start)->prev = Action->song_detail;
        }
        push(undostack, "add", Action->song_detail);
    }
    free(Action);
}
