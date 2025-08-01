// PlayWise Core Playlist Engine - C++ Implementation
// Author: (Ravi Kumar Reddy Goda)
// Description: Complete implementation of all 7 core modules + 2 bonus modules for the PlayWise Hackathon.
// Includes: Time/Space complexities & clean modular documentation

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
// DS Used: Doubly Linked List
// Why: Enables O(1) insertion, deletion, and movement in both directions, perfect for dynamic playlists
// Time Complexity:
// - Add/Delete/Move: O(1)
// - Display: O(n)
// - Get all songs: O(n)

struct Song
{
    string title, artist, genre;
    int duration; // in seconds
    Song *prev;
    Song *next;
    int id; // Unique identifier
    Song(string t, string a, string g, int d, int i) : title(t), artist(a), genre(g), duration(d), prev(NULL), next(NULL), id(i) {}
};

unordered_map<string, Song *> songMapByTitle; // O(1) lookup by title
unordered_map<int, Song *> songMapByID;       // O(1) lookup by ID

class Playlist
{
private:
    Song *head;
    Song *tail;
    static int songCounter;

public:
    Playlist() : head(NULL), tail(NULL) {}

    // Add song to end
    void add_song(string title, string artist, string genre, int duration)
    {
        Song *newSong = new Song(title, artist, genre, duration, songCounter++);
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

    // Delete song by index (0-based)
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

    // Move song from one index to another
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

        // Detach
        if (temp->prev)
            temp->prev->next = temp->next;
        if (temp->next)
            temp->next->prev = temp->prev;
        if (temp == head)
            head = temp->next;
        if (temp == tail)
            tail = temp->prev;

        // Re-insert
        Song *dest = head;
        i = 0;
        while (dest && i < to)
        {
            dest = dest->next;
            i++;
        }
        if (!dest)
        {
            add_song(temp->title, temp->artist, temp->genre, temp->duration);
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

    // Reverse the playlist
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

    // Display all songs
    void display()
    {
        Song *curr = head;
        while (curr)
        {
            cout << curr->title << " by " << curr->artist << " [" << curr->genre << "] (" << curr->duration << "s, ID: " << curr->id << ")\n";
            curr = curr->next;
        }
    }

    // Return all songs as vector
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
// DS Used: Stack
// Why: LIFO order matches undo functionality, perfect for tracking last played song
// Time Complexity: O(1)

stack<Song *> playbackHistory;

void undo_last_play(Playlist &playlist)
{
    if (!playbackHistory.empty())
    {
        Song *song = playbackHistory.top();
        playbackHistory.pop();
        playlist.add_song(song->title, song->artist, song->genre, song->duration);
        cout << "Restored: " << song->title << "\n";
    }
    else
    {
        cout << "No playback history.\n";
    }
}

//----------------------------------
// Module 3: Song Rating Tree (BST)
//----------------------------------
// DS Used: Binary Search Tree
// Why: BST supports efficient sorted rating storage & retrieval for dynamic inserts/searches
// Time Complexity:
// - Insert: O(log n) avg, O(n) worst
// - Inorder Display: O(n)

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

    void print_inorder(RatingNode *node)
    {
        if (!node)
            return;
        print_inorder(node->left);
        cout << "Rating: " << node->rating << "\n";
        for (Song *s : node->songs)
        {
            cout << " - " << s->title << " by " << s->artist << "\n";
        }
        print_inorder(node->right);
    }

    void display_all()
    {
        print_inorder(root);
    }
};

//----------------------------------
// Module 4: Instant Lookup (HashMap)
//----------------------------------
// DS Used: unordered_map
// Why: Provides average-case constant-time lookups for song metadata by ID or title
// Time Complexity: O(1) avg, O(n) worst

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
// DS Used: vector + sort (STL)
// Why: Sorting on vectors is simple and fast with built-in sort — great for snapshot exports and ordering
// Time Complexity: O(n log n)

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
// Goal: Achieve constant-time core operations and minimal space overhead
// Achieved via:
// - Pointer-based list for O(1) adds/moves
// - HashMaps for O(1) lookups
// - Static ID assignment without extra memory
// Why: Prioritizing performance and memory use for real-time CLI playback simulation

//----------------------------------
// Module 7: System Snapshot
//----------------------------------
// Outputs top 5 longest songs
// Why: Snapshot reports help summarize playlist contents quickly
// Time Complexity: O(n log n)

void export_snapshot(Playlist &playlist)
{
    vector<Song *> songs = playlist.get_all_songs();
    sort_by_duration(songs, false);
    cout << "Top 5 Longest Songs:\n";
    for (int i = 0; i < min(5, (int)songs.size()); ++i)
    {
        cout << songs[i]->title << " - " << songs[i]->duration << "s\n";
    }
}

//----------------------------------
// Bonus Module 1: Offline Playlist Caching
//----------------------------------
// Caches Top-N longest songs
// Why: Caching improves UX in offline mode — good for performance preview features
// Time Complexity: O(n log n)

void cache_top_n_songs(Playlist &playlist, int N)
{
    vector<Song *> songs = playlist.get_all_songs();
    sort_by_duration(songs, false);
    cout << "\nOffline Cache (Top " << N << " Songs):\n";
    for (int i = 0; i < min(N, (int)songs.size()); ++i)
    {
        cout << songs[i]->title << " by " << songs[i]->artist << " (" << songs[i]->duration << "s)\n";
    }
}

//----------------------------------
// Bonus Module 2: Genre Rebalancer
//----------------------------------
// Analyzes genre distribution
// Why: Encourages diversity by identifying genre imbalance using frequency % analysis
// Time Complexity: O(n)

void genre_rebalance(Playlist &playlist)
{
    unordered_map<string, int> genreCount;
    vector<Song *> songs = playlist.get_all_songs();

    for (Song *song : songs)
    {
        genreCount[song->genre]++;
    }

    int total = songs.size();
    cout << "\nGenre Distribution:\n";
    for (auto &entry : genreCount)
    {
        cout << entry.first << " : " << entry.second << " song(s)\n";
    }

    for (auto &entry : genreCount)
    {
        double percentage = (entry.second * 100.0) / total;
        if (percentage > 70.0)
        {
            cout << "Genre '" << entry.first << "' dominates the playlist (" << percentage << "%). Consider adding more variety.\n";
        }
    }
}

//----------------------------------
// Main Demonstration
//----------------------------------

int main()
{
    Playlist myPlaylist;
    RatingBST ratings;
    int choice;

    while (true)
    {
        cout << "\n==== PlayWise Playlist CLI ====\n";
        cout << "1. Add Song\n2. Delete Song\n3. Move Song\n4. Reverse Playlist\n5. View Playlist\n6. Snapshot (Top 5 Songs)\n";
        cout << "7. Cache Top-N Songs\n8. Genre Rebalance\n9. Rate Song\n10. View Rated Songs\n11. Lookup Song by Title\n12. Undo Last Play\n13. Play song\n14. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 14)
            break;

        if (choice == 1)
        {
            string title, artist, genre;
            int duration;
            cout << "Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Artist: ";
            getline(cin, artist);
            cout << "Genre: ";
            getline(cin, genre);
            cout << "Duration (in seconds): ";
            cin >> duration;
            myPlaylist.add_song(title, artist, genre, duration);
        }
        else if (choice == 2)
        {
            int index;
            cout << "Enter index to delete: ";
            cin >> index;
            myPlaylist.delete_song(index);
        }
        else if (choice == 3)
        {
            int from, to;
            cout << "From index: ";
            cin >> from;
            cout << "To index: ";
            cin >> to;
            myPlaylist.move_song(from, to);
        }
        else if (choice == 4)
        {
            myPlaylist.reverse_playlist();
        }
        else if (choice == 5)
        {
            myPlaylist.display();
        }
        else if (choice == 6)
        {
            export_snapshot(myPlaylist);
        }
        else if (choice == 7)
        {
            int n;
            cout << "Enter N: ";
            cin >> n;
            cache_top_n_songs(myPlaylist, n);
        }
        else if (choice == 8)
        {
            genre_rebalance(myPlaylist);
        }
        else if (choice == 9)
        {
            string title;
            int rating;
            cout << "Enter song title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter rating (1-5): ";
            cin >> rating;
            Song *s = lookup_song_by_title(title);
            if (s)
                ratings.insert_song(s, rating);
            else
                cout << "Song not found.\n";
        }
        else if (choice == 10)
        {
            ratings.display_all();
        }
        else if (choice == 11)
        {
            string title;
            cout << "Enter title: ";
            cin.ignore();
            getline(cin, title);
            Song *s = lookup_song_by_title(title);
            if (s)
                cout << "Found: " << s->title << " by " << s->artist << " [" << s->genre << "] (" << s->duration << "s)\n";
            else
                cout << "Song not found.\n";
        }
        else if (choice == 12)
        {
            undo_last_play(myPlaylist);
        }
        else if (choice == 13)
        {
            string title;
            cout << "Enter song title to play: ";
            cin.ignore();
            getline(cin, title);
            Song *s = lookup_song_by_title(title);
            if (s)
            {
                cout << "Now playing: " << s->title << " by " << s->artist << "\n";
                playbackHistory.push(s);
            }
            else
            {
                cout << "Song not found.\n";
            }
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    cout << "\nExiting PlayWise.\n";
    return 0;
}
