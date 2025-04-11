#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Node for linked list implementation
struct CourseNode {
    string courseName;
    string semester;
    char grade;
    CourseNode* next;
    
    CourseNode(string name, string sem, char g) : courseName(name), semester(sem), grade(g), next(nullptr) {}
};

// Course history linked list
class CourseHistory {
private:
    CourseNode* head;
    
public:
    CourseHistory() : head(nullptr) {}
    
    ~CourseHistory() {
        while (head != nullptr) {
            CourseNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void addCourse(string name, string semester, char grade) {
        CourseNode* newNode = new CourseNode(name, semester, grade);
        if (head == nullptr) {
            head = newNode;
        } else {
            CourseNode* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    
    void displayHistory() {
        if (head == nullptr) {
            cout << "No course history available." << endl;
            return;
        }
        
        cout << "\n=== Course History ===" << endl;
        CourseNode* current = head;
        while (current != nullptr) {
            cout << "Course: " << current->courseName 
                 << " | Semester: " << current->semester 
                 << " | Grade: " << current->grade << endl;
            current = current->next;
        }
    }
};

// Stack implementation for drop history
class DropHistory {
private:
    struct DropNode {
        string courseName;
        DropNode* next;
    };
    
    DropNode* top;
    
public:
    DropHistory() : top(nullptr) {}
    
    ~DropHistory() {
        while (top != nullptr) {
            DropNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    
    void push(string courseName) {
        DropNode* newNode = new DropNode{courseName, top};
        top = newNode;
    }
    
    string pop() {
        if (top == nullptr) {
            return "";
        }
        
        DropNode* temp = top;
        string courseName = temp->courseName;
        top = top->next;
        delete temp;
        return courseName;
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
};

// Queue implementation for course registration and waitlist
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
    };
    
    Node* front;
    Node* rear;
    int size;
    const int maxSize = 5; // Maximum courses a student can register for
    
public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
    
    bool enqueue(T item) {
        if (isFull()) {
            return false;
        }
        
        Node* newNode = new Node{item, nullptr};
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
        return true;
    }
    
    T dequeue() {
        if (isEmpty()) {
            return T();
        }
        
        Node* temp = front;
        T item = temp->data;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        return item;
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    bool isFull() {
        return size >= maxSize;
    }
    
    void display() {
        if (isEmpty()) {
            cout << "No courses registered." << endl;
            return;
        }
        
        Node* current = front;
        int index = 1;
        while (current != nullptr) {
            cout << index++ << ". " << current->data << endl;
            current = current->next;
        }
    }
    
    bool contains(T item) {
        Node* current = front;
        while (current != nullptr) {
            if (current->data == item) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    bool remove(T item) {
        if (isEmpty()) {
            return false;
        }
        
        Node* current = front;
        Node* prev = nullptr;
        
        while (current != nullptr) {
            if (current->data == item) {
                if (prev == nullptr) {
                    // Removing the first element
                    front = front->next;
                    if (front == nullptr) {
                        rear = nullptr;
                    }
                } else {
                    prev->next = current->next;
                    if (current == rear) {
                        rear = prev;
                    }
                }
                
                delete current;
                size--;
                return true;
            }
            
            prev = current;
            current = current->next;
        }
        
        return false;
    }
};

// Student class
class Student {
private:
    string studentId;
    string name;
    Queue<string> registeredCourses;
    CourseHistory courseHistory;
    DropHistory dropHistory;
    Queue<string> waitlist;
    
public:
    Student(string id, string n) : studentId(id), name(n) {}
    
    string getId() { return studentId; }
    string getName() { return name; }
    
    bool registerCourse(string courseName) {
        if (registeredCourses.contains(courseName)) {
            cout << "You are already registered for this course." << endl;
            return false;
        }
        
        if (registeredCourses.isFull()) {
            cout << "You have reached the maximum course load. Adding to waitlist." << endl;
            return waitlist.enqueue(courseName);
        }
        
        if (registeredCourses.enqueue(courseName)) {
            cout << "Successfully registered for " << courseName << endl;
            return true;
        }
        
        return false;
    }
    
    void viewRegisteredCourses() {
        cout << "\n=== Registered Courses ===" << endl;
        registeredCourses.display();
    }
    
    bool dropCourse(string courseName) {
        if (registeredCourses.remove(courseName)) {
            dropHistory.push(courseName);
            cout << "Successfully dropped " << courseName << endl;
            
            // Check if there's anyone in waitlist to enroll
            if (!waitlist.isEmpty()) {
                string nextCourse = waitlist.dequeue();
                registeredCourses.enqueue(nextCourse);
                cout << "Enrolled from waitlist: " << nextCourse << endl;
            }
            
            return true;
        }
        
        cout << "You are not registered for " << courseName << endl;
        return false;
    }
    
    void undoDrop() {
        string courseName = dropHistory.pop();
        if (courseName.empty()) {
            cout << "No recent drops to undo." << endl;
            return;
        }
        
        if (registeredCourses.isFull()) {
            cout << "Cannot undo drop. Course load is full." << endl;
            dropHistory.push(courseName); // Put it back
            return;
        }
        
        registeredCourses.enqueue(courseName);
        cout << "Successfully re-registered for " << courseName << endl;
    }
    
    void viewWaitlist() {
        cout << "\n=== Waitlisted Courses ===" << endl;
        if (waitlist.isEmpty()) {
            cout << "No courses in waitlist." << endl;
        } else {
            waitlist.display();
        }
    }
    
    void addToHistory(string courseName, string semester, char grade) {
        courseHistory.addCourse(courseName, semester, grade);
    }
    
    void viewCourseHistory() {
        courseHistory.displayHistory();
    }
};

// Course class
class Course {
private:
    string courseId;
    string name;
    int capacity;
    int enrolled;
    
public:
    Course(string id, string n, int cap) : courseId(id), name(n), capacity(cap), enrolled(0) {}
    
    string getId() { return courseId; }
    string getName() { return name; }
    
    bool isFull() {
        return enrolled >= capacity;
    }
    
    void incrementEnrollment() {
        enrolled++;
    }
    
    void decrementEnrollment() {
        if (enrolled > 0) {
            enrolled--;
        }
    }
    
    void displayInfo() {
        cout << "Course: " << name << " (" << courseId << ") | Enrolled: " << enrolled << "/" << capacity << endl;
    }
};

// Main system class
class CourseRegistrationSystem {
private:
    vector<Student> students;
    vector<Course> courses;
    
    Student* findStudent(string studentId) {
        for (auto& student : students) {
            if (student.getId() == studentId) {
                return &student;
            }
        }
        return nullptr;
    }
    
    Course* findCourse(string courseId) {
        for (auto& course : courses) {
            if (course.getId() == courseId) {
                return &course;
            }
        }
        return nullptr;
    }
    
    void displayAllCourses() {
        cout << "\n=== Available Courses ===" << endl;
        for (auto& course : courses) {
            course.displayInfo();
        }
    }
    
    void initializeSampleData() {
        // Add some sample courses
       
       
      courses.push_back(Course("ICT12050D", "Introduction to Programming", 10));
      courses.push_back(Course("ICT1203D", "Data Structures with C++", 20));
      courses.push_back(Course("ICT1204D", "Database system", 30));
      courses.push_back(Course("ICT12010D", "E-commerce", 40));
      courses.push_back(Course("ICT1205D", "System Administration", 50));
      courses.push_back(Course("ICT1201D","Computer Organization and Architecture",60));
      courses.push_back(Course("ICT12011D","Technopreneurship", 70));
        
        // Add some sample students
        students.push_back(Student("MRU2024000123", "Nambasa Moreen"));
        students.push_back(Student("MRU2024000920", "Lutaaya Rodyne"));
        students.push_back(Student("MRU2025000045", "Ahimbisiimbwe Tracy"));
        
        // Add some historical data for testing
        Student* s = findStudent("MRU2024000123");
        if (s) {
            s->addToHistory("ICT12010D", "Fall 2024", 'B');
            s->addToHistory("ICT1204D", "Fall 2024", 'A');
        }
    }
    
public:
    CourseRegistrationSystem() {
        initializeSampleData();
    }
    
    void run() {
        while (true) {
            cout << "\n=== Muteesa 1 Royal University Course Registration System ===" << endl;
            cout << "1. Student Login" << endl;
            cout << "2. View All Courses" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1: studentLogin(); break;
                case 2: displayAllCourses(); break;
                case 3: cout << "Exiting system. Goodbye!" << endl; return;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    
    void studentLogin() {
        string studentId;
        cout << "Enter your student ID: ";
        cin >> studentId;
        
        Student* student = findStudent(studentId);
        if (!student) {
            cout << "Student not found." << endl;
            return;
        }
        
        cout << "\nWelcome, " << student->getName() << "!" << endl;
        
        while (true) {
            cout << "\n=== Student Menu ===" << endl;
            cout << "1. Register for a course" << endl;
            cout << "2. View registered courses" << endl;
            cout << "3. Drop a course" << endl;
            cout << "4. Undo last drop" << endl;
            cout << "5. View waitlisted courses" << endl;
            cout << "6. View course history" << endl;
            cout << "7. Logout" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1: {
                    displayAllCourses();
                    string courseId;
                    cout << "Enter course ID to register: ";
                    cin >> courseId;
                    
                    Course* course = findCourse(courseId);
                    if (!course) {
                        cout << "Course not found." << endl;
                        break;
                    }
                    
                    if (student->registerCourse(course->getName())) {
                        course->incrementEnrollment();
                    }
                    break;
                }
                
                case 2:
                    student->viewRegisteredCourses();
                    break;
                
                case 3: {
                    student->viewRegisteredCourses();
                    string courseName;
                    cout << "Enter course name to drop: ";
                    cin.ignore();
                    getline(cin, courseName);
                    
                    if (student->dropCourse(courseName)) {
                        Course* course = findCourse(courseName.substr(0, 6)); // Extract course ID
                        if (course) {
                            course->decrementEnrollment();
                        }
                    }
                    break;
                }
                
                case 4:
                    student->undoDrop();
                    break;
                
                case 5:
                    student->viewWaitlist();
                    break;
                
                case 6:
                    student->viewCourseHistory();
                    break;
                
                case 7:
                    cout << "Logging out..." << endl;
                    return;
                
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

int main() {
    CourseRegistrationSystem system;
    system.run();
    return 0;
}