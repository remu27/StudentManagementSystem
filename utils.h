#ifndef UTILS_H
#define UTILS_H

#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 정렬 함수들
void sort_by_id(StudentManager *manager);
void sort_by_name(StudentManager *manager);
void sort_by_gpa(StudentManager *manager);

// 순위 계산 함수
void calculate_rankings(StudentManager *manager);

// 통계 함수들
float get_class_average_gpa(const StudentManager *manager);
float get_highest_gpa(const StudentManager *manager);
float get_lowest_gpa(const StudentManager *manager);
StudentRecord* get_top_student(StudentManager *manager);

// 과목별 통계 함수들
float get_subject_average(const StudentManager *manager, const char *subject);
int count_students_with_subject(const StudentManager *manager, const char *subject);
void display_subject_statistics(const StudentManager *manager, const char *subject);

// 출력 함수들
void display_ranking_report(const StudentManager *manager);
void display_class_statistics(const StudentManager *manager);

// 검색 함수들
void search_students_by_gpa_range(const StudentManager *manager, 
                                  float min_gpa, float max_gpa);
void search_students_by_grade(const StudentManager *manager, char grade);

// 비교 함수들 (qsort용)
int compare_by_id(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
int compare_by_gpa(const void *a, const void *b);

#endif // UTILS_H
