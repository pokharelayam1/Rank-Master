# **Rank Master** 🎓

**Rank Master** is a high-performance, single-executable web server built in **C++** that ranks students using a **Max-Heap** data structure. It features a modern, responsive web interface served directly from the backend logic, eliminating the need for separate external HTML or CSS files.

---

## 📊 **The Ranking Engine**
The core of Rank Master is a weighted scoring algorithm that prioritizes academic consistency. The system uses a **Max-Heap** (`std::priority_queue`) to ensure that the student with the highest calculated score is always at the "top" of the list.

### **Weighted Formula**
The program calculates the final ranking using the following weights:

$$FinalScore = (GPA \times 0.7) + (Marks \times 0.2) + (Attendance \times 0.1)$$

* **GPA (70%):** The primary driver of the rank.
* **Marks (20%):** Reflects testing performance.
* **Attendance (10%):** Accounts for student consistency.



---

## 💾 **Data Persistence & File Management**
To ensure that student records are not lost when the server is restarted, the system includes a built-in file management module using **C++ File Streams (`std::fstream`)**.

* **Automatic Logging:** Every time a student is added via the web interface, the record is instantly appended to a local file named `students_records.txt`.
* **Append Mode:** The system uses `ios::app` to ensure that new data is added to the end of the file without overwriting existing entries.
* **Format:** Data is stored in a structured, readable format:
  `Name: [Name] | GPA: [GPA] | Marks: [Marks] | Attendance: [Attendance]% | Final Score: [Score]`



---

## 🛠️ **Technical Stack**
* **Language:** C++11 or higher.
* **Networking:** **WinSock2** (Windows Sockets) for handling TCP/IP traffic and HTTP requests.
* **Data Structures:** * `std::priority_queue`: Implements the Max-Heap for $O(\log n)$ insertion and $O(1)$ access to the top student.
    * `std::stringstream`: Used for dynamic HTML generation.
    * `std::fstream`: Handles persistent data storage and retrieval.
* **Frontend:** HTML5 and CSS3 (Embedded as internal strings within the C++ source).



---

## ⚙️ **How It Works (Program Flow)**
1. **Socket Initialization:** The server starts and binds to `localhost:8080`.
2. **Request Handling:** * **GET Request:** The server sends the "Student Entry" form.
    * **POST Request:** The server extracts data from the body using a custom `getParam` parser.
3. **Heap Insertion:** A new `Student` object is created and pushed into the Priority Queue.
4. **Ranking:** The Max-Heap automatically re-orders the students based on the overloaded `<` operator in the `Student` struct.
5. **Storage:** The `saveToFile()` function writes the new record to `students_records.txt`.
6. **View Generation:** The server iterates through a copy of the heap to build the "Rankings" list in descending order.


----

## 🚀 **Installation & Usage**

### **Prerequisites**
* Windows Operating System.
* A C++ compiler (MinGW, GCC, or MSVC).

### **Compilation**
To compile via command line (MinGW), use the `-lws2_32` flag to link the Windows Socket library:
```bash
g++ main.cpp -o RankMaster.exe -lws2_32
```


### **Running the Program**
Run the executable directly from your terminal:
```bash
.\RankMaster.exe
```

### **Accessing the Interface**
Once the server is running, open any web browser and visit:
```bash
http://localhost:8080
```