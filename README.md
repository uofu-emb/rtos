# ECE 5785/6785 Advanced Embedded Systems

**Fall 2024**
Monday/Wednesday 11:50AM-01:10PM MEB 2265

Website: https://github.com/uofu-emb/rtos

Instructor:  Ashton Snelgrove

Email: ashton.snelgrove@utah.edu

Office Location:  SMBB 3750

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

See here for [full detailed learning objectives.](learning-objectives.pdf)

## Class format
- 5 minute case study presentation
- 20 minute discussion
- Remaining time working in the lab in pairs.

## Tooling
We will be using [Raspberry Pico](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html) toolchain to provide OS, HAL, and libraries. We will be using [FreeRTOS](htts://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/00-TaskHandle).

[VSCode](https://code.visualstudio.com/) is the recommended IDE. Most of the time we will be operating on the **command line**.

I recommend you use a Linux machine if possible. Linux is what you will be using in the industry, you need to be comfortable with it.

## Materials

You will need to purchase **two** Picos from the ECE stockroom (MEB 2355).

You will also need to purchase a CAN transceiver module from the stockroom later in the semester.

Total cost should be less than $15.

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

Labs include a reference implementation which you may find useful.

If you do find a code snippet, example online, or read the reference implementation - I recommend you type it into your code rather than copy and paste. This will make you engage with the code and give a better understanding.

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
| Grade | Graduate Requirements                                                                      | Undergraduate Requirements                                                                     |
|-------|--------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------|
| C     | Case study presentation.<br/>Labs 0-7<br/>Contribute to the final project.                 | Case study presentation.<br/>Labs 0-6<br/>Contribute to the final project. |
| B     | All neccessary to get a C, lab 8 and 2 more labs                                           | All neccessary to get a C and 2 more labs                                                        |
| A     | All neccessary to get a B and 2 more labs.<br/>Lead a team for a task during final project | All neccessary to get a B and 2 more labs                                                      |

Note that you are working in pairs, and a choice to abandon an assignment affects your partner.
# Assignments

## Safety case study with presentation
Each student is assigned a short, 5 minute presentation. This presentation will cover a case study of a critical failure of an embedded system. The case could come from any situation, including medical devices, transportation, industrial control, and home automation.  The presentation should discuss the consequences of the failure, the root causes, and what could have been done to prevent or mitigate the failure. At the beginning of class, one student will give their presentation.
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
| Day      | Lab                                      | Discussion                                                    |
|----------|------------------------------------------|---------------------------------------------------------------|
| Aug. 19  |                                          | Course syllabus, assign case study presentations. Git review. |
| Aug. 21  |                                          | Pull requests                                                 |
| Aug. 26  | Lab 0 hello world!                       | Real Time OS features, bare metal comparison.                 |
| Aug. 28  | Lab 1 Makefiles                          | Makefile basics                                               |
| Sept. 2  | Labor Day                                | Relax                                                         |
| Sept. 4  | Lab 2 Testing techniques                 | Testing techniques                                            |
| Sept. 9  | Lab 2 con't                              | refactoring                                                   |
| Sept. 11 | Lab 2 con't                              | writing for testability                                       |
| Sept. 16 | Lab 3 Tasks, Semaphores, Synchronization | Shared state,                                                 |
| Sept. 18 | Lab 3 con't                              | race conditions, deadlock                                     |
| Sept. 23 | Lab 4 Interprocess communication         | Communication patterns                                        |
| Sept. 25 | Lab 4 con't                              | Queues and backpressure                                       |
| Sept. 30 | Lab 9 Formal verification                | Model checking                                                |
| Oct. 2   | Lab 9 con't                              |                                                               |
| Oct. 7   | fall break                               |                                                               |
| Oct. 9   | fall break                               |                                                               |
| Oct. 14  | Lab 5 Real-time systems                  | Real Time requirements                                        |
| Oct. 16  | Lab 5 con't                              | Priority inversion                                            |
| Oct. 21  | Lab 8 CAN Bus                            | Networking                                                    |
| Oct. 23  | Lab 8 con't                              | real-time communication                                       |
| Oct. 28  | Lab 6 Scheduling                         | Scheduling algorithms                                         |
| Oct. 30  | Lab 6 con't                              | Prioritization                                                |
| Nov. 4   | Lab 10 Power                             | Measuring and estimating power                                |
| Nov. 6   | Lab 10 con't                             |                                                               |
| Nov. 11  | Lab 7 Error Handling                     | Safety, reliability, and risk                                 |
| Nov. 13  | Lab 7 con't                              | Networking fallacies                                          |
| Nov. 18  | Lab 11 Bluetooth                         | Radio technologies                                            |
| Nov. 20  | Lab 11 con't                             |                                                               |
| Nov. 25  | Lab 12 SMP                               | Context switching vs symmetric multiprocessing                |
| Nov. 27  | Thanksgiving Eve                         |                                                               |
| Dec. 2   | Lab 12 con't                             | Designing for concurrency                                     |
| Dec. 4   | Lab 12 con't                             |                                                               |

Exam period is Wednesday, December 13, 2023 3:30 – 5:30 pm. We will use this time to eat ice cream, discuss what happened during the course, and show off our results.

Note: This syllabus is meant to serve as an outline and guide for our course. Please note that I may modify it with reasonable notice to you. I may also modify the Course Schedule to accommodate the needs of our class. Any changes will be announced in class and posted on Canvas under Announcements.

# University Policies and Resources
- **The Americans with Disabilities Act**. The University of Utah seeks to provide equal access to its programs, services, and activities for people with disabilities. If you will need accommodations in this class, reasonable prior notice needs to be given to the Center for Disability & Access, 162 Olpin Union Building, (801) 581-5020. CDS will work with you and the instructor to make arrangements for accommodations. All written information in this course can be made available in an alternative format with prior notification to the Center for Disability & Access.
- **Attendance Accomodations** Given the nature of this course, attendance is required and adjustments cannot be granted to allow non-attendance. However, if you need to seek an ADA accommodation to request an exception to this attendance policy due to a disability, please contact the Center for Disability and Access (CDA). CDA will work with us to determine what, if any, ADA accommodations are reasonable and appropriate.
- **University Safety Statement**. The University of Utah values the safety of all campus community members. To report suspicious activity or to request a courtesy escort, call campus police at 801-585-COPS (801-585-2677). You will receive important emergency alerts and safety messages regarding campus safety via text message. For more information regarding safety and to view available training resources, including helpful videos, visit https://safeu.utah.edu
- **Addressing Discrimination, Harrasment, Sexual Misconduct**. Title IX makes it clear that violence and harassment based on sex and gender (which includes sexual orientation and gender identity/expression) is a civil rights offense subject to the same kinds of accountability and the same kinds of support applied to offenses against other protected categories such as race, national origin, color, religion, age, status as a person with a disability, veteran’s status or genetic information.  If you or someone you know has been harassed or assaulted, you are encouraged to report it to the [Title IX Coordinator](https://sexualassault.utah.edu/reporting/title-ix-on-campus-reporting/) in the Office of Equal Opportunity and Affirmative Action, 383 SOUTH UNIVERSITY STREET, LEVEL 1 OEO SUITE, 801-581-8365, or the Office of the Dean of Students, 270 Union Building, 801-581-7066.  For support and confidential consultation, contact the Center for Student Wellness, 426 SSB, 801-581-7776.  To report to the police, contact the Department of Public Safety, 801-585-2677(COPS].
- **Support Resources for Students**. Your success at the University of Utah is important to all of us here! If you feel like you need extra support in academics, overcoming personal difficulties, or finding community, we at the U are here for you. Recent legislation has [forced changes in the organization of the University student resources and restricts statements that may be made by faculty and staff](https://attheu.utah.edu/law-politics/how-will-utahs-2024-diversity-equity-and-inclusion-law-impact-campus/). Student resources have been consolidated under the [Student Affairs office](https://studentaffairs.utah.edu/), including the following resources.
    - [Center for Community & Cultural Engagement](https://culture.utah.edu/)
	- [Center for Student Access & Resources](https://studentresources.utah.edu/)
    - [Center for Native Excellence & Tribal Engagement](https://nativeexcellence.utah.edu/)
	- [Dream Center](https://dream.utah.edu/)
	- [Veterans Support Center](https://veterans.utah.edu/)
	- [Center for Campus Wellness](https://wellness.utah.edu/)
	- [Center for Disability & Access](https://disability.utah.edu/)
	- [Student Health Center](https://studenthealth.utah.edu/)
	- [Counseling Center](https://counselingcenter.utah.edu/)
	- [Basic Needs Collective](https://basicneeds.utah.edu/)
	- [International Student & Scholar Services](https://www.isss.utah.edu/)
    - Student resources and support programs previously managed by the LGBT Resource Center, Women's Resource Center, Black Cultural Center, and others will now be administered by the [Center for Student Access & Resources](https://studentresources.utah.edu/)!
- **Student Mental Health Resources**. Rates of burnout, anxiety, depression, isolation, and loneliness have noticeably increased during the pandemic. If you need help, reach out for campus mental health resources, including free counseling, trainings and other support. Consider participating in a Mental Health First Aid or other wellness-themed training provided by our Center for Student Wellness and sharing these opportunities with your peers, teaching assistants and department colleagues
- **Academic Misconduct Statement**. It is expected that students adhere to University of Utah policies regarding academic honesty, including but not limited to refraining from cheating, plagiarizing, misrepresenting one's work, and/or inappropriately collaborating. This includes the use of generative artificial intelligence (AI) tools without citation, documentation, or authorization. Students are expected to adhere to the prescribed professional and ethical standards of the profession/discipline for which they are preparing. Any student who engages in academic dishonesty or who violates the professional and ethical standards for their profession/discipline may be subject to academic sanctions as per the University of Utah’s [Student Code](https://regulations.utah.edu/academics/6-410.php)
- **Drop/Withdrawal Policies**. Students may drop a course within the first two weeks of a given semester without any penalties. Students may officially withdraw (W) from a class or all classes after the drop deadline through the midpoint of a course. A “W” grade is recorded on the transcript and appropriate tuition/fees are assessed. The grade “W” is not used in calculating the student’s GPA. For deadlines to withdraw from full-term, first, and second session classes, see the U's Academic Calendar.
