#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct playlist
{
    char song_title[20];
    char song_artist[20];
    int duration;
    struct playlist *next;
    struct playlist *prev;
};
struct palylist *start=NULL;

struct playlist *create(struct playlist *start);
struct playlist *deleted(struct playlist *start);

int main()
{
    do
    {
        int choice;
        printf("-----Music Playlist-----\n");
        printf("Press Number:- \n1) Create/Insert \n2)Delete 3)Exit\n");
        printf("------------------------\n");
        printf("Enter Choice: -");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            start=create(start);
            break;
        case 2:
            start=deleted(start);
            break;
        case 3:
            printf("\n-----Exiting-----\n");
            break;
        default:
            printf("\nInvalid Choice Enter\n")
        }
    }
    while(choice!=3);
    return 0;
}
struct playlist *create(struct playlist *start)
{
    int time = 0;
    printf("\nEnter -1 for Exit");
    while(time!=1)
    {
        printf("\nEnter the duration of the song: - ");
        scanf("%d",&time);
        if(time==-1)
        {
            return start;
        }
        char artist_name[20],song_name[20];
        printf("\nEnter the name of the artist: - ");
        gets(artist_name);
        printf("\nEnter the name of the song: - ");
        gets(song_name);
        struct playlist *new_song,*ptr;
        new_song=(struct playlist *)malloc(sizeof(struct playlist));
        if(!new_song)
        {
            printf("\nMemory overflow \n");
            return start;
        }
        new_song->duration=time;
        strcpy(new_song->song_artist,artist_name);
        strcpy(new_song->song_title,song_name);
        new_song->next=NULL;
        new_song->prev=NULL;
        if(start==NULL)
        {
            start=new_song;
        }
        else
        {
            ptr=start;
            while(ptr->next!=NULL)
                ptr=ptr->next;
            ptr->next=new_song;
            new_song->prev=ptr;
        }
    }
    return start;
}

struct playlist *deleted(struct playlist *start)
{
    if(start==NULL)
        printf("\n Playlist is empty");
        return start;
    else
    {
        struct playlist *ptr;
        ptr=start;
        char song_name[20];
        printf("\nEnter song title: -");
        gets(song_name);
        while(ptr!=NULL)
        {
            if(strcmp(ptr->song_title,song_name))
            {
                ptr->prev->next = ptr->next;
                ptr->next->prev=ptr->prev;
                free(ptr);
                return start;
            }
            else
                ptr=ptr->next;
        }
        return start;
    }
