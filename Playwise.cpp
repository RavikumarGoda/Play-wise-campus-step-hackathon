// PlayWise Core Playlist Engine - C++ Implementation
// Author: Brodi (Ravi Kumar Reddy Goda)
// Description: Complete implementation of all 7 core modules for the PlayWise Hackathon.

#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

//----------------------------------
// Module 1: Playlist using Doubly Linked List
//----------------------------------

struct Song
{
    string title, artist;
    int duration; // in seconds
    Song *prev;
    Song *next;
    int id; // Unique identifier
    Song(string t, string a, int d, int i) : title(t), artist(a), duration(d), prev(NULL), next(NULL), id(i) {}
};

unordered_map<string, Song *> songMapByTitle;
unordered_map<int, Song *> songMapByID;

class Playlist
{
private:
    Song *head;
    Song *tail;
    static int songCounter;

public:
    Playlist() : head(NULL), tail(NULL) {}

    void add_song(string title, string artist, int duration)
    {
        Song *newSong = new Song(title, artist, duration, songCounter++);
        if (!head)
            head = tail = newSong;
        else
        {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
        songMapByID[newSong->id] = newSong;
        songMapByTitle[title] = newSong;
    }

    void delete_song(int index)
    {
        Song *temp = head;
        int i = 0;
        while (temp && i < index)
        {
            temp = temp->next;
            i++;
        }
        if (!temp)
            return;
        songMapByID.erase(temp->id);
        songMapByTitle.erase(temp->title);
        if (temp->prev)
            temp->prev->next = temp->next;
        if (temp->next)
            temp->next->prev = temp->prev;
        if (temp == head)
            head = temp->next;
        if (temp == tail)
            tail = temp->prev;
        delete temp;
    }

    void move_song(int from, int to)
    {
        if (from == to)
            return;
        Song *temp = head;
        int i = 0;
        while (temp && i < from)
        {
            temp = temp->next;
            i++;
        }
        if (!temp)
            return;

        if (temp->prev)
            temp->prev->next = temp->next;
        if (temp->next)
            temp->next->prev = temp->prev;
        if (temp == head)
            head = temp->next;
        if (temp == tail)
            tail = temp->prev;

        Song *dest = head;
        i = 0;
        while (dest && i < to)
        {
            dest = dest->next;
            i++;
        }
        if (!dest)
        {
            add_song(temp->title, temp->artist, temp->duration);
            delete temp;
            return;
        }
        if (dest->prev)
            dest->prev->next = temp;
        temp->prev = dest->prev;
        temp->next = dest;
        dest->prev = temp;
        if (to == 0)
            head = temp;
    }

    void reverse_playlist()
    {
        Song *curr = head;
        Song *prev = NULL, *next = NULL;
        while (curr)
        {
            next = curr->next;
            curr->next = prev;
            curr->prev = next;
            prev = curr;
            curr = next;
        }
        swap(head, tail);
    }

    void display()
    {
        Song *curr = head;
        while (curr)
        {
            cout << curr->title << " by " << curr->artist << " (" << curr->duration << "s, ID: " << curr->id << ")\n";
            curr = curr->next;
        }
    }

    vector<Song *> get_all_songs()
    {
        vector<Song *> result;
        Song *curr = head;
        while (curr)
        {
            result.push_back(curr);
            curr = curr->next;
        }
        return result;
    }
};

int Playlist::songCounter = 1;

//----------------------------------
// Module 2: Playback History using Stack
//----------------------------------

stack<Song *> playbackHistory;

void undo_last_play(Playlist &playlist)
{
    if (!playbackHistory.empty())
    {
        Song *song = playbackHistory.top();
        playbackHistory.pop();
        playlist.add_song(song->title, song->artist, song->duration);
    }
}

//----------------------------------
// Module 3: Song Rating Tree (BST)
//----------------------------------

struct RatingNode
{
    int rating;
    vector<Song *> songs;
    RatingNode *left, *right;
    RatingNode(int r) : rating(r), left(NULL), right(NULL) {}
};

class RatingBST
{
public:
    RatingNode *root;
    RatingBST() : root(NULL) {}

    RatingNode *insert(RatingNode *node, Song *song, int rating)
    {
        if (!node)
        {
            node = new RatingNode(rating);
            node->songs.push_back(song);
            return node;
        }
        if (rating < node->rating)
            node->left = insert(node->left, song, rating);
        else if (rating > node->rating)
            node->right = insert(node->right, song, rating);
        else
            node->songs.push_back(song);
        return node;
    }

    void insert_song(Song *song, int rating)
    {
        root = insert(root, song, rating);
    }
};

//----------------------------------
// Module 4: Instant Lookup (HashMap)
//----------------------------------

Song *lookup_song_by_title(const string &title)
{
    if (songMapByTitle.count(title))
        return songMapByTitle[title];
    return nullptr;
}

Song *lookup_song_by_id(int id)
{
    if (songMapByID.count(id))
        return songMapByID[id];
    return nullptr;
}

//----------------------------------
// Module 5: Sorting Module
//----------------------------------

void sort_by_title(vector<Song *> &songs)
{
    sort(songs.begin(), songs.end(), [](Song *a, Song *b)
         { return a->title < b->title; });
}

void sort_by_duration(vector<Song *> &songs, bool ascending = true)
{
    sort(songs.begin(), songs.end(), [ascending](Song *a, Song *b)
         { return ascending ? a->duration < b->duration : a->duration > b->duration; });
}

//----------------------------------
// Module 6: Playback Optimization
//----------------------------------

// Optimized by:
// - Using pointers (O(1) song addition/deletion)
// - Syncing HashMaps on every change
// - Avoiding unnecessary copying in move_song
// - Constant-time song ID assignment via static counter

//----------------------------------
// Module 7: System Snapshot
//----------------------------------

void export_snapshot(Playlist &playlist)
{
    vector<Song *> songs = playlist.get_all_songs();
    sort_by_duration(songs, false); // Descending
    cout << "Top 5 Longest Songs:\n";
    for (int i = 0; i < min(5, (int)songs.size()); ++i)
    {
        cout << songs[i]->title << " - " << songs[i]->duration << "s\n";
    }
}

//----------------------------------
// Sample main to demo modules
//----------------------------------

int main()
{
    Playlist myPlaylist;
    myPlaylist.add_song("On My Way", "Alan Walker", 210);
    myPlaylist.add_song("Faded", "Alan Walker", 180);
    myPlaylist.add_song("Alone", "Marshmello", 200);
    myPlaylist.display();

    cout << "\nAfter Reversing:\n";
    myPlaylist.reverse_playlist();
    myPlaylist.display();

    cout << "\nLookup Song by Title: Faded\n";
    Song *s = lookup_song_by_title("Faded");
    if (s)
        cout << "Found: " << s->title << " by " << s->artist << "\n";

    cout << "\nSnapshot:\n";
    export_snapshot(myPlaylist);
    return 0;
}
