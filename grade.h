#ifndef GRADE_H
#define GRADE_H

#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 성적 관리 함수들
int add_grade(StudentRecord *record, const char *subject, 
              float score, int credit);
int update_grade(StudentRecord *record, const char *subject, float new_score);
int delete_grade(StudentRecord *record, const char *subject);
Grade* search_grade(StudentRecord *record, const char *subject);

// 성적 계산 함수들
float calculate_gpa(StudentRecord *record);
char score_to_grade(float score);
float calculate_average_score(const StudentRecord *record);
float calculate_total_credits(const StudentRecord *record);

// 성적 출력 함수들
void display_transcript(const StudentRecord *record);
void display_grade_summary(const StudentRecord *record);

// 유틸리티 함수들
int is_valid_score(float score);
int is_valid_credit(int credit);
int has_subject(const StudentRecord *record, const char *subject);

#endif // GRADE_H
