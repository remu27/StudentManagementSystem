#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "student.h"
#include "grade.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 기본 파일명
#define DEFAULT_DATA_FILE "students.csv"
#define BACKUP_FILE "students_backup.csv"

// CSV 구분자
#define CSV_DELIMITER ","
#define GRADE_DELIMITER ";"

// 파일 저장/로드 함수
int save_data(const StudentManager *manager, const char *filename);
int load_data(StudentManager *manager, const char *filename);

// 백업 관련 함수
int create_backup(const char *source_file);
int restore_from_backup(StudentManager *manager);

// 파일 검증 함수
int file_exists(const char *filename);
int is_valid_csv_format(const char *filename);

// CSV 파싱 함수들
char* trim_whitespace(char *str);
int parse_student_line(const char *line, StudentRecord *record);
int parse_grades_string(const char *grades_str, StudentRecord *record);

// 내보내기/가져오기 함수
int export_to_csv(const StudentManager *manager, const char *filename);
int import_from_csv(StudentManager *manager, const char *filename);

#endif // FILE_HANDLER_H
