# 🎵 PlayWise Core Playlist Engine

Modular, interactive CLI playlist manager built in modern C++ — developed for the PlayWise Hackathon.

---

## ⚙️ Tech Stack

- **Language**: C++
- **Paradigm**: Object-Oriented Programming
- **Data Structures Used**:
  - Doubly Linked List (for playlist)
  - Stack (for playback history)
  - Binary Search Tree (for song ratings)
  - Hash Maps (`unordered_map`) for fast lookup
  - Vectors for storage and sorting

> ⚡ No external libraries used. Fully standard C++ STL based.

---

## 🚀 Features

| Module | Description |
|--------|-------------|
| **1. Playlist Engine** | Doubly linked list structure for efficient song addition, deletion, and movement. |
| **2. Playback History** | Stack-based undo functionality to restore the last played song. |
| **3. Rating System (BST)** | Songs can be rated 1–5 and stored in a binary search tree for structured browsing. |
| **4. Instant Lookup** | Fast O(1) search for songs by title or ID using hash maps. |
| **5. Sorting Module** | Sort songs by title or duration (ascending/descending). |
| **6. Playback Optimization** | Optimized operations using pointer manipulation for constant-time access and reordering. |
| **7. Snapshot Report** | View top 5 longest songs as a quick summary. |

---

## 🏅 Bonus Modules

| Module | Description |
|--------|-------------|
| **Offline Caching** | Cache top-N longest songs for offline mode. |
| **Genre Rebalancer** | Analyze and display genre distribution in your playlist. |
| **Play Song** | Simulates song playback and pushes it to playback history. |

---

## 🎮 CLI Menu (Interactive)

```txt
==== PlayWise Playlist CLI ====

1. Add Song  
2. Delete Song  
3. Move Song  
4. Reverse Playlist  
5. View Playlist  
6. Snapshot (Top 5 Songs)  
7. Cache Top-N Songs  
8. Genre Rebalance  
9. Rate Song  
10. View Rated Songs  
11. Lookup Song by Title  
12. Undo Last Play  
13. Play Song  
14. Exit
