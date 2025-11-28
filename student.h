#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 상수 정의
#define MAX_NAME_LEN 50
#define MAX_MAJOR_LEN 50
#define MAX_STUDENTS 100
#define MAX_SUBJECTS 10

// 학생 기본 정보 구조체
typedef struct {
    int id;                      // 학번
    char name[MAX_NAME_LEN];     // 이름
    char major[MAX_MAJOR_LEN];   // 전공
    char reg_date[11];           // 등록일 (YYYY-MM-DD)
} Student;

// 과목 성적 구조체
typedef struct {
    char subject[MAX_NAME_LEN];  // 과목명
    float score;                 // 점수 (0.0 ~ 100.0)
    char grade;                  // 학점 등급 (A, B, C, D, F)
    int credit;                  // 학점 (1 ~ 4)
} Grade;

// 학생 전체 레코드 구조체
typedef struct {
    Student info;                // 학생 기본 정보
    Grade grades[MAX_SUBJECTS];  // 성적 배열
    int subject_count;           // 등록된 과목 수
    float gpa;                   // 평균 학점
} StudentRecord;

// 전체 학생 관리 구조체
typedef struct {
    StudentRecord *records;      // 학생 레코드 배열 (동적 할당)
    int count;                   // 현재 학생 수
    int capacity;                // 최대 수용 가능 학생 수
} StudentManager;

// 학생 관리 함수들
StudentManager* create_manager(int initial_capacity);
void destroy_manager(StudentManager *manager);
int add_student(StudentManager *manager, int id, const char *name, 
                const char *major, const char *reg_date);
int delete_student(StudentManager *manager, int id);
StudentRecord* search_student_by_id(StudentManager *manager, int id);
StudentRecord* search_student_by_name(StudentManager *manager, const char *name);
void display_student(const StudentRecord *record);
void display_all_students(const StudentManager *manager);

// 유틸리티 함수들
int is_duplicate_id(const StudentManager *manager, int id);
void update_student_info(StudentRecord *record, const char *name, 
                        const char *major, const char *reg_date);

#endif // STUDENT_H
