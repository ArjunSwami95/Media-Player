#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <time.h>

#define GRADIENT 4 // we need whole numbers for represeting songs in array. 
#define MAX_RATING 5 //assuming maximum rating for any song is 5

struct Song
{
    float rating;
    int song_id;
};
struct MediaPlayer
{
    struct Song *songsList;
    int song_counter;
};
int getSize(float rating) //to get the number of indices each song would require
{
    return rating*GRADIENT;
}
void addSong(struct MediaPlayer *mediaPlayer, struct Song song)
{
    struct Song *songs = mediaPlayer->songsList;
    int idx = mediaPlayer->song_counter;
    float rating = song.rating;
    int size = getSize(rating); // 1.25 would take a size of 5 since 
    int i = 0;
    for(i = idx; i<idx+size; i++)
    {
        songs[i] = song;
    }
    mediaPlayer->song_counter = i;
}
void openMediaPlayer(struct MediaPlayer *mediaPlayer, struct Song songs[],int n)
{
    printf("media player has been opened.......\n");
    mediaPlayer->song_counter = 0;
    int arrSize = n*MAX_RATING*GRADIENT;//assuming all songs present in the list have full ratings
    mediaPlayer->songsList = (struct Song*)malloc(sizeof(struct Song)*arrSize);
    for(int i = 0;i<n;i++)
        addSong(mediaPlayer,songs[i]);
}
void closeMediaPlayer(struct MediaPlayer *mediaPlayer)
{
    mediaPlayer->song_counter = -1;
    struct Song *songs = mediaPlayer->songsList;
    free(songs);
    mediaPlayer = NULL;
    printf("media player has been closed.........");
}
void deleteSong(struct MediaPlayer *mediaPlayer, struct Song song)
{
    int song_id = song.song_id;
    float rating = song.rating;
    int songSize = getSize(rating);
    int currentSongListSize = mediaPlayer->song_counter;
    int newSongListSize = (currentSongListSize - songSize);// if original array is of size 40 and a song deleted is of size 10 (rating 2.5) then new array size needed is 30
    struct Song* currentList = mediaPlayer->songsList;
    struct Song* newList = (struct Song*)malloc(sizeof(struct Song)*newSongListSize);
    int counter = 0;
 
    for(int i = 0; i<currentSongListSize; i++)
    {
        if(currentList[i].song_id!=song_id)
        {
            newList[counter++]  = currentList[i];  
        }
    }
    
    mediaPlayer->songsList = newList;
    mediaPlayer->song_counter = counter;
    free(currentList);
    return;
}

int getNextSong(struct MediaPlayer *mediaPlayer)
{
    if(mediaPlayer==NULL || mediaPlayer->song_counter<=0) // when the songs array is empty
        return -1;
    
    int size = mediaPlayer->song_counter;
    srand(time(NULL));// using random function to get a random index everytime getnextsSong() is called.
    int idx = rand()%size; 
    struct Song *songs = mediaPlayer->songsList;
    int song_id =songs[idx].song_id;
    deleteSong(mediaPlayer,songs[idx]);
    return song_id; 
}

void printSongs(struct MediaPlayer *mediaPlayer) // for testting/troubleshooting puposes.
{
    if(mediaPlayer==NULL)
    {
        printf("list is empty");  
        return;       
    }
    struct Song *songs = mediaPlayer->songsList;
    int size = mediaPlayer->song_counter;
    
    if(size == 0)
        printf(" list has 0 songs to play ");
    for(int i = 0;i<size; i++)
    {
        printf("%d\t",songs[i].song_id);
        printf("%f\n",songs[i].rating);
    }
}

int main()
 {
    int id = -1;
    struct MediaPlayer mediaPlayer;
    struct Song song;
    song.song_id = 8;
    song.rating = 1.25;
    struct Song song2;
    song2.song_id = 7;
    song2.rating = 2.25;
    struct Song songs[2];
    songs[0] = song;
    songs[1] = song2;
    
    openMediaPlayer(&mediaPlayer, songs,2); // we add 2 songs to the media player.
    printSongs(&mediaPlayer); // songs are printed with their ratings.
    id = getNextSong(&mediaPlayer);//random song is generated and id is checked.
    printf("\nsong_id = %d\n",id);
    printSongs(&mediaPlayer);// checked if remaining list is correctly handled.
    id = getNextSong(&mediaPlayer);// further next song is requested.
    printf("\nsong_id = %d\n",id);
    printSongs(&mediaPlayer);// to test the case when list is empty.
    id = getNextSong(&mediaPlayer);// to test the case when the list is empty.
    printf("\nsong_id = %d\n",id);
    closeMediaPlayer(&mediaPlayer);
    return 0;
    
}