# ECE 5960/6960 Advanced Embedded Systems

**Fall 2023**
Monday 15:00-16:05, Wedneday 15:00-16:20 MEB 2265

Website: https://github.com/uofu-emb/rtos

Instructor:  Ashton Snelgrove

Email: ashton.snelgrove@utah.edu

Office Location:  MEB 1339

## Course Description

This is a follow up course in embedded systems, focusing on real-time requirements and reliable systems. The course will cover practical implementation of real-time systems using Real Time Operating Systems (RTOS). Topics covered include: timing requirements, low power modes and energy efficiency, concurrency, distributed communication using CAN bus, system testing and verification.

## Course Outcomes

At the end of this course, you should:

- Be able to build
    - safe and fault tolerant system
    - real-time systems
    - distributed systems.
    - concurrent systems
    - low-power systems
- Work in an industry environment:
    - Setup a project from scratch
    - Write automated tests.
    - Use source control.
    - Review code.
    - Modify existing code base.

See here for [full detailed learning objectives.](learning_objectives.pdf)

## Class format
- 5 minute case study presentation
- 20 minute discussion
- Remaining time working in the lab in pairs.

## Tooling
We will be using [PlatformIO](https://platformio.org) to provide toolchain, OS, HAL, and libraries. We will be using [Zephyr RTOS](https://docs.zephyrproject.org/2.7.5/) (note version 2.7.5).

[VSCode](https://code.visualstudio.com/) is the recommended IDE. Most of the time we will be operating on the command line.

## Cheating and ethics

__We learn good practice by seeing good practice.__

__We learn good practice by critiquing bad practice.__

This is a collaborative course, you are not just encouraged but required to work together!

- What is not cheating:
    - Reading examples on the internet.
    - Working with other students
    - Reading other student's code.
    - Helping debug
    - Working with people from outside class.
- What is cheating:
    - Copying code without understanding it.

Students are expected to maintain the ethical and professional standards of the engineering discipline. Show respect for one another. For more information see the University Policies section below.

## Attendance & Punctuality

This is a collaborative and lab based course. Students are required to attend each class and participate. Please let me know as soon as possible if you cannot come to class. If you are sick, stay home!

## Course Policies

Instructor will respond to emails within 24 hours.

Students are expected to complete code reviews within **3 business days of being assigned**. Students should make arrangements with lab partners to meet outside of class time if required.

### Grading Policy (Evaluation Methods & Criteria)

#### Specifications Grading (Nielson 2014)

- Goal of assignments is to demonstrate competency and mastery of a topic.
- Assignments are graded pass/fail. No partial credit.
- Each assignment will have a detailed specification that is required to be met.
- Unacceptable assignments can be resubmitted.

#### Code assignment requirements:
All code must:
- pass comprehensive automated tests written by you.
- pass any automated tests provided by us.
- be committed to source control using git.
- peer reviewed

### Letter Grades
Letter grades are assigned based on completion of the following:
| Grade | Graduate Requirements | Undergraduate Requirements |
| --- | --- | --- |
| C | Case study presentation.<br/>Labs 0, 1, 2, 3, 4, and 5<br/>Contribute to the final project.| Case study presentation.<br/>Labs 0, 1, 2, 3, and 4<br/>Contribute to the final project.|
| B | All neccessary to get a C, lab 6 and 1 more labs | All neccessary to get a C, labs 5 and 6 |
| A | All neccessary to get a B and 2 more labs.<br/>Lead a team for a task during final project | All neccessary to get a B and 2 more labs |

# Assignments

## Safety case study with presentation
Each student is assigned a short, 5 minute presentation. This presentation will cover a case study of a critical failure of an embedded system. The case could come from any situation, including medical devices, transportation, industrial control, and home automation. The presentation should discuss the consequences of the failure, the root causes, and what could have been done to prevent or mitigate the failure. At the beginning of class, one student will give their presentation.
## Labs
Labs are broken into several components. You must complete all components of the lab.
- Pre-lab primer
    - The pre-lab also includes introductory material. This may include videos and articles to review before class.
    - The pre-lab includes a set of questions. Students will submit their answers on Canvas the day before class.
- Lab coding activities
    - All lab activities will be done in pairs (if an odd number of students is present, one team will have three members).
    - The instructor and TA will be present for the entire lab session to discuss and provide feedback.
    - Each lab includes a set of activities with explanatory introduction. All activities are intended to yield unit tests that validate the outcome of the activity.
- Post-lab reflection survey
    - A post-lab survey is given. This helps the students evaluate their own performance and gives the instructor an opportunity to evaluate the outcomes of the lab.
    - The survey is administered on Canvas.
- Code review
    - When all activities are complete, the team will create a pull request (PR). Each team will be paired with another team. Each team will be responsible for doing code review for the other team. In order to pass the code review, all unit tests must pass. The reviewers should verify the code  completes all the requirements of the lab. The team should also review for style, organization, and clarity. If changes are necessary, the team can make the changes and resubmit for review.
    - In order to complete the lab, students must pass their own code review. They must also complete their assigned review. One unavoidable problem of the review process is that control is given to another party during review. It is the team's responsibility to ensure the reviewing team finishes the review. Badgering your colleague to finish reviewing your PR is a fact of life in industry practice. If needed, the instructor can help resolve any conflicts.
## Project to synthesize concepts
The class will work together on a large project to apply the concepts learned in the course. Small groups will take on tasks that contribute towards the final product. Students will work on multiple tasks and teams over the course of the project.

# Course Schedule
| Day | Lab | Discussion |
|-----------|----------|---------|
| Aug. 21 |  | Course syllabus, assign case study presentations. Git review, pull requests. |
| Aug. 23 | Lab 0 hello world! | Real Time OS features, bare metal comparison.  |
| Aug. 28 | Lab 1 Testing techniques | Testing techniques, refactoring, writing for testability |
| Aug. 30 | Lab 2 Mocking | Function binding hijinks |
| Sept. 4| Labor Day | Relax |
| Sept. 6  | Lab 3 Tasks, Semaphores, Synchronization | Shared state, race conditions, deadlock |
| Sept. 11 | Lab 4 Interprocess communication | Communication patterns |
| Sept. 13 | Lab 4 con't | Non-blocking patterns |
| Sept. 18.|Lab 5 Real-time systems | Real Time requirements |
| Sept. 20| Lab 5 con't |  |
| Sept. 25|  Lab 6 Scheduling | scheduling, prioritization |
| Sept. 27 |Lab 6 con't | Priority inversion |
| Oct. 2 | Lab 7 CAN Bus | Networking |
| Oct. 4 |  Lab 7 con't| real-time communication |
| Oct. 9 | fall break | |
| Oct. 11 | fall break | |
| Oct. 16 | Lab 8 Error handling | Safety, network, reliability (start case study presentations) |
| Oct. 18 | Lab 8 con't | |
| Oct. 23 | Lab 9 Power |Measuring and estimating power|
| Oct. 25 | Lab 9 con't | Event driven systems. |
| Oct. 30 | Project begins | |
| Nov. 1 | | |
| Nov. 6 | | |
| Nov. 8 | | |
| Nov. 13 | | |
| Nov. 15 | | |
| Nov. 20 | | |
| Nov. 22 |Thanksgiving Eve | |
| Nov. 27 | | |
| Nov. 29 | | |
| Dec. 4 | | |
| Dec. 6 | | |

Exam period is Wednesday, December 13, 2023 3:30 – 5:30 pm. We will use this time to discuss what happened during the course, and show off our results.

Note: This syllabus is meant to serve as an outline and guide for our course. Please note that I may modify it with reasonable notice to you. I may also modify the Course Schedule to accommodate the needs of our class. Any changes will be announced in class and posted on Canvas under Announcements.

# University Policies
- **The Americans with Disabilities Act**. The University of Utah seeks to provide equal access to its programs, services, and activities for people with disabilities. If you will need accommodations in this class, reasonable prior notice needs to be given to the Center for Disability & Access, 162 Olpin Union Building, (801) 581-5020. CDS will work with you and the instructor to make arrangements for accommodations. All written information in this course can be made available in
an alternative format with prior notification to the Center for Disability & Access.
- **ADA accomodation** Given the nature of this course, attendance is required and adjustments cannot be granted to allow non-attendance. However, if you need to seek an ADA accommodation to request an exception to this attendance policy due to a disability, please contact the Center for Disability and Access (CDA). CDA will work with us to determine what, if any, ADA accommodations are reasonable and appropriate 
- **University Safety Statement**. The University of Utah values the safety of all campus community members. To report suspicious activity or to request a courtesy escort, call campus police at 801-585-COPS (801-585-2677). You will receive important emergency alerts and safety messages regarding campus safety via text message. For more information regarding safety and to view available training resources, including helpful videos, visit https://safeu.utah.edu
- **Addressing Sexual Misconduct**. Title IX makes it clear that violence and harassment based on sex and gender (which Includes sexual orientation and gender identity/expression) is a civil rights offense subject to the same kinds of accountability and the same kinds of support applied to offenses against other protected categories such as race, national origin, color, religion, age, status as a person with a disability, veteran’s status or genetic information.  If you or someone you know has been harassed or assaulted, you are encouraged to report it to the Title IX Coordinator in the Office of Equal Opportunity and Affirmative Action, 135 Park Building, 801-581-8365, or the Office of the Dean of Students, 270 Union Building, 801-581-7066.  For support and confidential consultation, contact the Center for Student Wellness, 426 SSB, 801-581-7776.  To report to the police, contact the Department of Public Safety, 801-585-2677(COPS).
- **Lauren’s Promise**: Lauren’s Promise is a vow that anyone – faculty, staff, students, parents, and community members – can take to indicate to others that they represent a safe haven for sharing incidents of sexual assault, domestic violence, or stalking. Anyone who makes Lauren’s Promise vows to: 1.) listen to and believe those individuals who are being threatened or experiencing sexual assault, dating violence or stalking; 2.) represent a safe haven for sharing incidents of sexual assault, domestic violence, or stalking; and 3.) change campus culture that responds poorly to dating violence and stalking. By making Lauren’s Promise, individuals are helping to change campus cultures that respond poorly to dating violence and stalking throughout the nation.
- **Academic Misconduct Statement**. It is expected that students adhere to University of Utah policies regarding academic honesty, including but not limited to refraining from cheating, plagiarizing, misrepresenting one's work, and/or inappropriately collaborating. This includes the use of generative artificial intelligence (AI) tools without citation, documentation, or authorization. Students are expected to adhere to the prescribed professional and ethical standards of the profession/discipline for which they are preparing. Any student who engages in academic dishonesty or who violates the professional and ethical standards for their profession/discipline may be subject to academic sanctions as per the University of Utah’s Student Code: https://regulations.utah.edu/academics/6-410.php
- **Diversity Statement**. I stand in support of compassion, dignity, value-of-life, equity, inclusion and justice for all individuals regardless of color, race/ethnicity, sexual orientation, religion, language, socioeconomic status, ability, gender, gender identity or expression, immigration status, or any type of marginalization. I stand in support of making our society more inclusive, just, and equitable for all individuals. I stand against individual and systemic racism in all its various forms.
- **Undocumented Student Support Statement**. Immigration is a complex phenomenon with broad impact—those who are directly affected by it, as well as those who are indirectly affected by their relationships with family members, friends, and loved ones. If your immigration status presents obstacles to engaging in specific activities or fulfilling specific course criteria, confidential arrangements may be requested from the Dream Center. Arrangements with the Dream Center will not jeopardize your student status, your financial aid, or any other part of your residence. The Dream Center offers a wide range of resources to support undocumented students (with and without DACA) as well as students from mixed-status families. To learn more, please contact the Dream Center at 801.213.3697 or visit https://dream.utah.edu.
- **Drop/Withdrawal Policies**. Students may drop a course within the first two weeks of a given semester without any penalties. Students may officially withdraw (W) from a class or all classes after the drop deadline through the midpoint of a course. A “W” grade is recorded on the transcript and appropriate tuition/fees are assessed. The grade “W” is not used in calculating the student’s GPA. For deadlines to withdraw from full-term, first, and second session classes, see the U's Academic Calendar.     •    Student Code: http://regulations.utah.edu/academics/6-400.php
- **Accommodation Policy** (see Section Q): http://regulations.utah.edu/academics/6-100.php
- **Student Mental Health Resources**. Rates of burnout, anxiety, depression, isolation, and loneliness have noticeably increased during the pandemic. If you need help, reach out for campus mental health resources, including free counseling, trainings and other support. Consider participating in a Mental Health First Aid or other wellness-themed training provided by our Center for Student Wellness and sharing these opportunities with your peers, teaching assistants and department colleagues
- **Diverse Supports for Students**. Your success at the University of Utah is important to all of us here! If you feel like you need extra support in academics, overcoming personal difficulties, or finding community, the U is here for you.
- **Student Support Services (TRIO)**
  TRIO federal programs are targeted to serve and assist low-income individuals, first-generation college students, and individuals with disabilities.
  Student Support Services (SSS) is a TRIO program for current or incoming undergraduate university students who are seeking their first bachelor's degree and need academic assistance and other services to be successful at the University of Utah.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Student Support Services (TRIO) 801-581-7188 https://trio.utah.edu  Room 2075       1901 E. S. Campus Dr.      Salt Lake City, UT 84112
- **American Indian Students**
  The AIRC works to increase American Indian student visibility and success on campus by advocating for and providing student centered programs and tools to enhance academic success, cultural events to promote personal well-being, and a supportive “home-away-from-home” space for students to grow and develop leadership skills.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  American Indian Resource Center  801-581-7019 https://diversity.utah.edu/centers/airc Fort Douglas Building 622      1925 De Trobriand St.      Salt Lake City, UT 84113
- **Black Students**
  Using a pan-African lens, the Black Cultural Center seeks to counteract persistent campus-wide and global anti-blackness. The Black Cultural Center works to holistically enrich, educate, and advocate for students, faculty, and staff through Black centered programming, culturally affirming educational initiatives, and retention strategies.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Black Cultural Center 801-213-1441 https://diversity.utah.edu/centers/bcc Fort Douglas Building 603      95 Fort Douglas Blvd.      Salt Lake City, UT 84113
- **Students with Children**
  Our mission is to support and coordinate information, program development and services that enhance family resources as well as the availability, affordability and quality of child care for University students, faculty and staff.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Center for Childcare & Family Resources 801-585-5897 https://childcare.utah.edu 408 Union Building     200 S. Central Campus Dr.      Salt Lake City, UT 84112
- **Students with Disabilities**
  The Center for Disability & Access is dedicated to serving students with disabilities by providing the opportunity for success and equal access at the University of Utah. They also strive to create an inclusive, safe, and respectful environment.
  For more information about what support they provide and links to other resources, view their website or contact:
  Center for Disability & Access 801-581-5020 https://disability.utah.edu 162 Union Building     200 S. Central Campus Dr.      Salt Lake City, UT 84112
- ***Students across Intersectional Identities and Experiences**
  The Center for Equity & Student Belonging (CESB) creates community and advocates for academic success and belonging for students across inter-sectional identities and experiences among our African, African American, Black, Native, Indigenous, American Indian, Asian, Asian American, Latinx, Chicanx, Pacific Islander, Multiracial, LGBTQ+, Neurodiverse and Disabled students of color.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Center for Equity and Student Belonging (CESB)  801-581-8151 https://diversity.utah.edu/centers/CESB/  235 Union Building     200 S. Central Campus Dr.      Salt Lake City, UT 84112
- **English as a Second/Additional Language (ESL) Students**
  If you are an English language learner, there are several resources on campus available to help you develop your English writing and language skills. Feel free to contact:
  Writing Center 801-587-9122 https://writingcenter.utah.edu 2701 Marriott Library      295 S 1500 E      Salt Lake City, UT 84112
  English Language Institute 801-581-4600 https://continue.utah.edu/eli  540 Arapeen Dr.      Salt Lake City, UT 84108
- **Undocumented Students**
  Immigration is a complex phenomenon with broad impact—those who are directly affected by it, as well as those who are indirectly affected by their relationships with family members, friends, and loved ones. If your immigration status presents obstacles that prevent you from engaging in specific activities or fulfilling specific course criteria, confidential arrangements may be requested from the Dream Center.
  Arrangements with the Dream Center will not jeopardize your student status, your financial aid, or any other part of your residence. The Dream Center offers a wide range of resources to support undocumented students (with and without DACA) as well as students from mixed-status families.
  For more information about what support they provide and links to other resources, view their website or contact:
  Dream Center 801-213-3697  https://dream.utah.edu  1120 Annex (Wing B)      1901 E. S. Campus Dr.      Salt Lake City, UT 84112
- **LGBTQ+ Students**
  The LGBTQ+ Resource Center acts in accountability with the campus community by identifying the needs of people with a queer range of [a]gender and [a]sexual experiences and responding with university-wide services.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  LGBTQ+ Resource Center 801-587-7973 https://lgbt.utah.edu (Links to an external site.) 409 Union Building     200 S. Central Campus Dr.      Salt Lake City, UT 84112
- **Veterans & Military Students**
  The mission of the Veterans Support Center is to improve and enhance the individual and academic success of veterans, service members, and their family members who attend the university; to help them receive the benefits they earned; and to serve as a liaison between the student veteran community and the university.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Veterans Support Center 801-587-7722 https://veteranscenter.utah.edu  (Links to an external site.) 418 Union Building     200 S. Central Campus Dr.      Salt Lake City, UT 84112
- **Women**
  The Women’s Resource Center (WRC) at the University of Utah serves as the central resource for educational and support services for women. Honoring the complexities of women’s identities, the WRC facilitates choices and changes through programs, counseling, and training grounded in a commitment to advance social justice and equality.
  For more information about what support they provide, a list of ongoing events, and links to other resources, view their website or contact:
  Women's Resource Center 801-581-8030 https://womenscenter.utah.edu 411 Union Building      200 S. Central Campus Dr.      Salt Lake City, UT 84112
- **Inclusivity at the U**
  The Office for Inclusive Excellence is here to engage, support, and advance an environment fostering the values of respect, diversity, equity, inclusivity, and academic excellence for students in our increasingly global campus community. They also handle reports of bias in the classroom as outlined below:
  Bias or hate incidents consist of speech, conduct, or some other form of expression or action that is motivated wholly or in part by prejudice or bias whose impact discriminates, demeans, embarrasses, assigns stereotypes, harasses, or excludes individuals because of their race, color, ethnicity, national origin, language, sex, size, gender identity or expression, sexual orientation, disability, age, or religion.
  For more information about what support they provide and links to other resources, or to report a bias incident, view their website or contact:
  Office for Inclusive Excellence 801-581-4600 https://inclusive-excellence.utah.edu (Links to an external site.) 170 Annex (Wing D)      1901 E. S. Campus Dr.      Salt Lake City, UT 84112
- **Other Student Groups at the U**
  To learn more about some of the other resource groups available at the U, check out:
  https://getinvolved.utah.edu/
  https://studentsuccess.utah.edu/resources/student-support
