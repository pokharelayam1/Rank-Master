#ifndef FRONTEND_H
#define FRONTEND_H

#include <string>

// Internal CSS for the Glassmorphism UI
const std::string HTML_STYLE = R"(
<style>
    body {
        font-family: 'Segoe UI', sans-serif;
        background: linear-gradient(135deg, #667eea, #764ba2);
        min-height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
        margin: 0;
    }
    .container {
        background: rgba(255, 255, 255, 0.95);
        padding: 30px;
        border-radius: 20px;
        box-shadow: 0 15px 35px rgba(0, 0, 0, 0.2);
        width: 100%;
        max-width: 400px;
    }
    h1 { text-align: center; color: #333; margin-bottom: 20px; }
    .input-group { margin-bottom: 15px; }
    label { 
        display: block; font-size: 11px; font-weight: bold; 
        color: #777; margin-bottom: 5px; text-transform: uppercase; 
    }
    input { 
        width: 100%; padding: 12px; border: 2px solid #eee; 
        border-radius: 10px; box-sizing: border-box; font-size: 14px;
    }
    input:focus { border-color: #764ba2; outline: none; }
    button { 
        width: 100%; padding: 14px; background: #764ba2; color: white; 
        border: none; border-radius: 10px; cursor: pointer; 
        font-weight: bold; font-size: 16px; transition: 0.3s;
    }
    button:hover { background: #5a3a82; }
    .student-item { 
        background: #f8f9fa; margin: 10px 0; padding: 15px; 
        border-radius: 12px; display: flex; justify-content: space-between; 
        align-items: center; border-left: 5px solid #764ba2; 
    }
    .score-badge { 
        background: #ece6ff; color: #764ba2; padding: 5px 12px; 
        border-radius: 20px; font-weight: bold; font-size: 13px; 
    }
    .back-btn { 
        display: block; text-align: center; margin-top: 20px; 
        color: #764ba2; text-decoration: none; font-weight: bold; 
    }
</style>
)";

// The Entry Form HTML
const std::string ENTRY_FORM = R"(
<h1>Rank Master</h1>
<form method='POST'>
    <div class='input-group'><label>Student Name</label><input type='text' name='name' required></div>
    <div class='input-group'><label>GPA (0.0 - 4.0)</label><input type='text' name='gpa' required></div>
    <div class='input-group'><label>Total Marks</label><input type='text' name='marks' required></div>
    <div class='input-group'><label>Attendance %</label><input type='text' name='attn' required></div>
    <button type='submit'>Calculate Rank</button>
</form>
)";

#endif