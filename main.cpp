#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <winsock2.h>
#include "frontend.h" // Import UI constants

#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct Student {
    string name;
    float gpa, marks, attendance, finalScore;
    // Max-Heap logic: Highest finalScore gets priority
    bool operator<(const Student& other) const { return finalScore < other.finalScore; }
};

priority_queue<Student> pq;

float calculateScore(float gpa, float marks, float attendance) {
    // Formula: GPA(70%) + Marks(20%) + Attendance(10%)
    return (gpa * 0.7f) + (marks * 0.2f) + (attendance * 0.1f);
}

string getParam(string body, string key) {
    size_t start = body.find(key + "=");
    if (start == string::npos) return "";
    start += key.length() + 1;
    size_t end = body.find("&", start);
    string val = body.substr(start, end - start);
    for (char &c : val) if (c == '+') c = ' '; // Decode URL spaces
    return val;
}

string generateHTML(bool showResults) {
    stringstream res;
    res << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        << "<html><head><meta charset='UTF-8'>" << HTML_STYLE << "</head><body><div class='container'>";

    if (!showResults) {
        res << ENTRY_FORM;
    } else {
        res << "<h1>Leaderboard</h1>";
        priority_queue<Student> temp = pq; // Copy heap to display
        while (!temp.empty()) {
            Student s = temp.top();
            res << "<div class='student-item'><span>" << s.name << "</span>"
                << "<span class='score-badge'>Score: " << s.finalScore << "</span></div>";
            temp.pop();
        }
        res << "<a href='/' class='back-btn'>+ Add Another Student</a>";
    }

    res << "</div></body></html>";
    return res.str();
}

int main() {
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server = {AF_INET, htons(8080), INADDR_ANY};
    
    bind(server_socket, (sockaddr*)&server, sizeof(server));
    listen(server_socket, 3);

    cout << "Rank Master is running!" << endl;
    cout << "Open your browser at http://localhost:8080" << endl;

    while (true) {
        SOCKET client = accept(server_socket, NULL, NULL);
        char buf[4096] = {0};
        recv(client, buf, 4096, 0);
        string req(buf);

        if (req.find("POST") != string::npos) {
            string b = req.substr(req.find("\r\n\r\n") + 4);
            try {
                string n = getParam(b, "name");
                float g = stof(getParam(b, "gpa"));
                float m = stof(getParam(b, "marks"));
                float a = stof(getParam(b, "attn"));
                pq.push({n, g, m, a, calculateScore(g, m, a)});
            } catch (...) { /* Handle parsing errors silently */ }
            
            string html = generateHTML(true);
            send(client, html.c_str(), (int)html.length(), 0);
        } else {
            string html = generateHTML(false);
            send(client, html.c_str(), (int)html.length(), 0);
        }
        closesocket(client);
    }
    return 0;
}