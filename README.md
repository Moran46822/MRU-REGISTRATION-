Explaining the code

1. Student Course Registration (Queue Implementation)
   - Implemented in the `Queue` template class and used in the `Student` class
   - Students can register (enqueue), view (display), and drop (remove) courses
   - The queue maintains registration order (FIFO principle)

2. Course Waitlist (Queue Implementation)
   - A separate `Queue` instance in the `Student` class handles waitlisted courses
   - When a course is dropped, the first student in the waitlist is automatically enrolled
   - Maximum course load (5 courses) enforced before adding to waitlist

3. Course Drop History (Stack Implementation)
   - Implemented in the `DropHistory` class (stack)
   - When a student drops a course, it's pushed onto the stack
   - Undo feature pops from the stack and re-enrolls the student

4. Student Course Records (Linked List Implementation)
   - Implemented in the `CourseHistory` class using a singly linked list
   - Each `CourseNode` stores course name, semester, and grade
   - Full history can be displayed through the student menu

- Data Validation:
  - Prevents duplicate course registrations
  - Handles waitlist enrollment when courses are full
  - Validates student IDs and course IDs
  - Prevents invalid menu choices

- User Interaction:
  - Menu-driven interface using `switch-case` and loops
  - Clear prompts and feedback for all operations
  - Handles invalid input gracefully

How to Compile and Run

1. Save the code as `Muteesa 1 Royal University.cpp`
2. Compile with: Cxx diode or C++ compiler Muteesa 1 Royal University.cpp. Code for course_registration`
3. Run the program still with any provided compiler above.



