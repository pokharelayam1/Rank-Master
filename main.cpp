#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Student {
    string name;
    float gpa, marks, attendance, finalScore;
    bool operator<(const Student& other) const { return finalScore < other.finalScore; }
};

priority_queue<Student> pq;

float calculateScore(float gpa, float marks, float attendance) {
    return (gpa * 0.7f) + (marks * 0.2f) + (attendance * 0.1f);
}

string getParam(string body, string key) {
    size_t start = body.find(key + "=");
    if (start == string::npos) return "";
    start += key.length() + 1;
    size_t end = body.find("&", start);
    string val = body.substr(start, end - start);
    for (char &c : val) if (c == '+') c = ' ';
    return val;
}

string generateHTML(bool showResults) {
    string style = 
        "<style>"
        "body{font-family:'Segoe UI',sans-serif; background:linear-gradient(135deg, #667eea, #764ba2); min-height:100vh; display:flex; justify-content:center; align-items:center; margin:0;}"
        ".container{background:white; padding:30px; border-radius:20px; box-shadow:0 15px 35px rgba(0,0,0,0.2); width:400px;}"
        "h1{text-align:center; color:#333; margin-top:0;} .input-group{margin-bottom:15px;}"
        "label{display:block; font-size:12px; font-weight:bold; color:#666; margin-bottom:5px; text-transform:uppercase;}"
        "input{width:100%; padding:10px; border:2px solid #eee; border-radius:8px; box-sizing:border-box; transition:0.3s;}"
        "input:focus{border-color:#764ba2; outline:none;}"
        "button{width:100%; padding:12px; background:#764ba2; color:white; border:none; border-radius:8px; cursor:pointer; font-weight:bold; font-size:16px; transition:0.3s;}"
        "button:hover{background:#5a3a82;} .rank-list{list-style:none; padding:0;}"
        ".student-item{background:#f8f9fa; margin:8px 0; padding:12px; border-radius:10px; display:flex; justify-content:space-between; align-items:center; border-left:5px solid #764ba2;}"
        ".score-badge{background:#ece6ff; color:#764ba2; padding:4px 10px; border-radius:20px; font-weight:bold; font-size:14px;}"
        "a{display:block; text-align:center; margin-top:15px; color:#764ba2; text-decoration:none; font-weight:bold;}"
        "</style>";

    stringstream body;
    if (!showResults) {
        body << "<h1>Student Entry</h1>"
             << "<form method='POST'>"
             << "<div class='input-group'><label>Student Name</label><input type='text' name='name' required></div>"
             << "<div class='input-group'><label>GPA (0.0 - 4.0)</label><input type='text' name='gpa' required></div>"
             << "<div class='input-group'><label>Marks</label><input type='text' name='marks' required></div>"
             << "<div class='input-group'><label>Attendance %</label><input type='text' name='attn' required></div>"
             << "<button type='submit'>Rank Student</button></form>";
    } else {
        body << "<h1>Rankings</h1><div class='rank-list'>";
        priority_queue<Student> temp = pq;
        while (!temp.empty()) {
            Student s = temp.top();
            body << "<div class='student-item'><span>" << s.name << "</span><span class='score-badge'>" << s.finalScore << "</span></div>";
            temp.pop();
        }
        body << "</div><a href='/'>+ Add Another</a>";
    }

    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
           "<html><head><meta charset='UTF-8'>" + style + "</head><body><div class='container'>" + body.str() + "</div></body></html>";
}

int main() {
    WSADATA wsa; 
    WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server = {AF_INET, htons(8080), INADDR_ANY};
    
    bind(server_socket, (sockaddr*)&server, sizeof(server));
    listen(server_socket, 3);

    cout << "Server active at http://localhost:8080" << endl;

    while (true) {
        SOCKET client = accept(server_socket, NULL, NULL);
        char buf[4096] = {0};
        recv(client, buf, 4096, 0);
        string req(buf);

        if (req.find("POST") != string::npos) {
            string b = req.substr(req.find("\r\n\r\n") + 4);
            string n = getParam(b, "name");
            float g = stof(getParam(b, "gpa"));
            float m = stof(getParam(b, "marks"));
            float a = stof(getParam(b, "attn"));
            
            pq.push({n, g, m, a, calculateScore(g, m, a)});
            string html = generateHTML(true);
            send(client, html.c_str(), (int)html.length(), 0);
        } else {
            string html = generateHTML(false);
            send(client, html.c_str(), (int)html.length(), 0);
        }
        closesocket(client);
    }
    
    closesocket(server_socket);
    WSACleanup();
    return 0;
}