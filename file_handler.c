#include "file_handler.h"

// 문자열 앞뒤 공백 제거
char* trim_whitespace(char *str) {
    char *end;
    
    // 앞쪽 공백 제거
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
        str++;
    }
    
    if (*str == 0) return str;
    
    // 뒤쪽 공백 제거
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    *(end + 1) = '\0';
    
    return str;
}

// 파일 존재 여부 확인
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// CSV 형식 유효성 검사 (기본적인 검사)
int is_valid_csv_format(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[1024];
    int valid = 0;
    
    // 첫 줄 읽기 (헤더)
    if (fgets(line, sizeof(line), file)) {
        // 최소한의 필드가 있는지 확인
        if (strstr(line, "ID") && strstr(line, "Name")) {
            valid = 1;
        }
    }
    
    fclose(file);
    return valid;
}

// 성적 문자열 파싱 (형식: "과목명:점수:학점;과목명:점수:학점;...")
int parse_grades_string(const char *grades_str, StudentRecord *record) {
    if (!grades_str || strlen(grades_str) == 0) {
        record->subject_count = 0;
        record->gpa = 0.0;
        return 1;
    }
    
    char *grades_copy = strdup(grades_str);
    char *grade_token = strtok(grades_copy, ";");
    int count = 0;
    
    while (grade_token && count < MAX_SUBJECTS) {
        char subject[MAX_NAME_LEN];
        float score;
        int credit;
        
        // "과목명:점수:학점" 형식 파싱
        if (sscanf(grade_token, "%[^:]:%f:%d", subject, &score, &credit) == 3) {
            strncpy(record->grades[count].subject, subject, MAX_NAME_LEN - 1);
            record->grades[count].subject[MAX_NAME_LEN - 1] = '\0';
            record->grades[count].score = score;
            record->grades[count].credit = credit;
            record->grades[count].grade = score_to_grade(score);
            count++;
        }
        
        grade_token = strtok(NULL, ";");
    }
    
    free(grades_copy);
    record->subject_count = count;
    record->gpa = calculate_gpa(record);
    
    return 1;
}

// CSV 한 줄 파싱하여 StudentRecord 생성
int parse_student_line(const char *line, StudentRecord *record) {
    char line_copy[2048];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';
    
    // CSV 필드 추출
    char *token;
    int field = 0;
    char *saveptr;
    
    token = strtok_r(line_copy, ",", &saveptr);
    
    while (token && field < 5) {
        token = trim_whitespace(token);
        
        switch (field) {
            case 0: // ID
                record->info.id = atoi(token);
                break;
            case 1: // Name
                strncpy(record->info.name, token, MAX_NAME_LEN - 1);
                record->info.name[MAX_NAME_LEN - 1] = '\0';
                break;
            case 2: // Major
                strncpy(record->info.major, token, MAX_MAJOR_LEN - 1);
                record->info.major[MAX_MAJOR_LEN - 1] = '\0';
                break;
            case 3: // Reg_date
                strncpy(record->info.reg_date, token, 10);
                record->info.reg_date[10] = '\0';
                break;
            case 4: // Grades
                parse_grades_string(token, record);
                break;
        }
        
        field++;
        token = strtok_r(NULL, ",", &saveptr);
    }
    
    return (field >= 4); // 최소 4개 필드는 있어야 함
}

// 데이터를 CSV 파일로 저장
int save_data(const StudentManager *manager, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "오류: 파일 '%s'를 열 수 없습니다.\n", filename);
        return 0;
    }
    
    // CSV 헤더 작성
    fprintf(file, "ID,Name,Major,Reg_Date,Grades\n");
    
    // 각 학생 데이터 저장
    for (int i = 0; i < manager->count; i++) {
        StudentRecord *record = &manager->records[i];
        
        // 기본 정보 작성
        fprintf(file, "%d,%s,%s,%s,",
                record->info.id,
                record->info.name,
                record->info.major,
                record->info.reg_date);
        
        // 성적 정보 작성 (형식: "과목명:점수:학점;과목명:점수:학점")
        if (record->subject_count > 0) {
            fprintf(file, "\"");
            for (int j = 0; j < record->subject_count; j++) {
                fprintf(file, "%s:%.1f:%d",
                        record->grades[j].subject,
                        record->grades[j].score,
                        record->grades[j].credit);
                
                if (j < record->subject_count - 1) {
                    fprintf(file, ";");
                }
            }
            fprintf(file, "\"");
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("데이터가 '%s'에 저장되었습니다. (총 %d명)\n", filename, manager->count);
    return 1;
}

// CSV 파일에서 데이터 로드
int load_data(StudentManager *manager, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "오류: 파일 '%s'를 열 수 없습니다.\n", filename);
        return 0;
    }
    
    char line[2048];
    int loaded_count = 0;
    
    // 헤더 라인 건너뛰기
    if (!fgets(line, sizeof(line), file)) {
        fprintf(stderr, "오류: 파일이 비어있거나 형식이 잘못되었습니다.\n");
        fclose(file);
        return 0;
    }
    
    // 기존 데이터 초기화
    manager->count = 0;
    
    // 각 라인 읽고 파싱
    while (fgets(line, sizeof(line), file) && manager->count < manager->capacity) {
        // 빈 줄 건너뛰기
        if (strlen(trim_whitespace(line)) == 0) continue;
        
        StudentRecord *record = &manager->records[manager->count];
        
        if (parse_student_line(line, record)) {
            manager->count++;
            loaded_count++;
        } else {
            fprintf(stderr, "경고: 라인 %d 파싱 실패\n", loaded_count + 2);
        }
    }
    
    fclose(file);
    printf("데이터가 '%s'에서 로드되었습니다. (총 %d명)\n", filename, loaded_count);
    return 1;
}

// 백업 파일 생성
int create_backup(const char *source_file) {
    if (!file_exists(source_file)) {
        fprintf(stderr, "경고: 원본 파일 '%s'이(가) 존재하지 않습니다.\n", source_file);
        return 0;
    }
    
    FILE *source = fopen(source_file, "r");
    FILE *backup = fopen(BACKUP_FILE, "w");
    
    if (!source || !backup) {
        fprintf(stderr, "오류: 백업 파일 생성 실패\n");
        if (source) fclose(source);
        if (backup) fclose(backup);
        return 0;
    }
    
    // 파일 내용 복사
    char buffer[1024];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, backup);
    }
    
    fclose(source);
    fclose(backup);
    
    printf("백업 파일 '%s'이(가) 생성되었습니다.\n", BACKUP_FILE);
    return 1;
}

// 백업에서 복원
int restore_from_backup(StudentManager *manager) {
    if (!file_exists(BACKUP_FILE)) {
        fprintf(stderr, "오류: 백업 파일 '%s'이(가) 존재하지 않습니다.\n", BACKUP_FILE);
        return 0;
    }
    
    printf("백업 파일에서 복원 중...\n");
    return load_data(manager, BACKUP_FILE);
}

// 데이터를 CSV로 내보내기 (사용자 지정 파일명)
int export_to_csv(const StudentManager *manager, const char *filename) {
    printf("데이터를 '%s'로 내보내는 중...\n", filename);
    return save_data(manager, filename);
}

// CSV에서 데이터 가져오기 (기존 데이터에 추가)
int import_from_csv(StudentManager *manager, const char *filename) {
    if (!file_exists(filename)) {
        fprintf(stderr, "오류: 파일 '%s'이(가) 존재하지 않습니다.\n", filename);
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "오류: 파일을 열 수 없습니다.\n");
        return 0;
    }
    
    char line[2048];
    int imported_count = 0;
    int skipped_count = 0;
    
    // 헤더 건너뛰기
    fgets(line, sizeof(line), file);
    
    // 현재 학생 수 저장
    int original_count = manager->count;
    
    // 각 라인 읽고 추가
    while (fgets(line, sizeof(line), file)) {
        if (strlen(trim_whitespace(line)) == 0) continue;
        
        StudentRecord temp_record;
        if (parse_student_line(line, &temp_record)) {
            // 중복 학번 검사
            if (is_duplicate_id(manager, temp_record.info.id)) {
                fprintf(stderr, "경고: 학번 %d는 이미 존재합니다. 건너뜀.\n", 
                        temp_record.info.id);
                skipped_count++;
                continue;
            }
            
            // 용량 확인
            if (manager->count >= manager->capacity) {
                fprintf(stderr, "경고: 최대 용량 초과. 더 이상 추가할 수 없습니다.\n");
                break;
            }
            
            // 학생 추가
            manager->records[manager->count] = temp_record;
            manager->count++;
            imported_count++;
        }
    }
    
    fclose(file);
    
    printf("\n가져오기 완료:\n");
    printf("- 추가된 학생: %d명\n", imported_count);
    printf("- 건너뛴 학생: %d명\n", skipped_count);
    printf("- 현재 총 학생 수: %d명\n", manager->count);
    
    return 1;
}
