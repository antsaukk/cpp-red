#include <string>
#include <deque>
#include <list>
#include <cmath>
#include "test_runner.h"
#include "profile.h"
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() {}

  void Left() {
    if (position != editor_state.begin()) { position = prev(position); }
  }
  void Right() {
    if (position != editor_state.end()) { position = next(position); }
  }

  void Insert(char token) {
    if (position == editor_state.end()) {
      editor_state.push_back(token);
      position = editor_state.end();
    } else {
      editor_state.insert(position, token);
    }
  }

  void Cut(size_t tokens = 1) {
    size_t count = 0; 
    while(count < tokens && position != editor_state.end()) {
      buffer.push_back(*position); 
      auto temp = position; 
      position = next(position);
      editor_state.erase(temp);
      count++;
    }
  }
  void Copy(size_t tokens = 1) {
    size_t count = 0;
    while(count < tokens && position != editor_state.end()) {
      buffer.push_back(*position); 
      position = next(position);
      count++;
    }
  }

  void Paste() {
    size_t count = 0; 
    while (count < buffer.size()) {
      char ch = buffer.front(); 
      buffer.pop_front();
      Insert(ch);
      buffer.push_back(ch);
      count++;
    }
  }

  string GetText() const {
    string s;
    s.reserve(Size());
    for(const auto& ch : editor_state) {
      s+=ch;
    }
    return s;
  }

  size_t Size() const { return editor_state.size(); }

  list<char>::iterator GetPos() { return position; }

private:
  list<char> editor_state;
  list<char>::iterator position = editor_state.begin();
  deque<char> buffer;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world"); 
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len); 
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    } 
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit"); 
    editor.Left();
    editor.Left();
    editor.Left(); 
    editor.Cut(1); 
    editor.Right();  
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

void BufferNoClean()
{
  Editor editor;
  TypeText(editor, "1234567");
  editor.Left();
  editor.Left();
  editor.Cut(1);
  editor.Paste();
  editor.Paste();
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "123456667");
}

int main() {
  TestRunner tr;
  LOG_DURATION("Total tests duration");
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  RUN_TEST(tr, BufferNoClean);
  return 0;
}