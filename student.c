#include "student.h"

// StudentManager 생성 - 초기 용량만큼 메모리 할당
StudentManager* create_manager(int initial_capacity) {
    StudentManager *manager = (StudentManager*)malloc(sizeof(StudentManager));
    if (!manager) {
        fprintf(stderr, "메모리 할당 실패: StudentManager\n");
        return NULL;
    }
    
    manager->records = (StudentRecord*)malloc(sizeof(StudentRecord) * initial_capacity);
    if (!manager->records) {
        fprintf(stderr, "메모리 할당 실패: StudentRecord 배열\n");
        free(manager);
        return NULL;
    }
    
    manager->count = 0;
    manager->capacity = initial_capacity;
    
    return manager;
}

// StudentManager 메모리 해제
void destroy_manager(StudentManager *manager) {
    if (manager) {
        if (manager->records) {
            free(manager->records);
        }
        free(manager);
    }
}

// 학번 중복 체크 - 중복이면 1, 아니면 0 반환
int is_duplicate_id(const StudentManager *manager, int id) {
    for (int i = 0; i < manager->count; i++) {
        if (manager->records[i].info.id == id) {
            return 1;
        }
    }
    return 0;
}

// 새 학생 추가 - 성공 시 1, 실패 시 0 반환
int add_student(StudentManager *manager, int id, const char *name, 
                const char *major, const char *reg_date) {
    // 용량 초과 검사
    if (manager->count >= manager->capacity) {
        fprintf(stderr, "오류: 최대 학생 수 초과 (최대: %d)\n", manager->capacity);
        return 0;
    }
    
    // 중복 학번 검사
    if (is_duplicate_id(manager, id)) {
        fprintf(stderr, "오류: 학번 %d는 이미 존재합니다.\n", id);
        return 0;
    }
    
    // 새 학생 레코드 초기화
    StudentRecord *new_record = &manager->records[manager->count];
    new_record->info.id = id;
    strncpy(new_record->info.name, name, MAX_NAME_LEN - 1);
    new_record->info.name[MAX_NAME_LEN - 1] = '\0';
    strncpy(new_record->info.major, major, MAX_MAJOR_LEN - 1);
    new_record->info.major[MAX_MAJOR_LEN - 1] = '\0';
    strncpy(new_record->info.reg_date, reg_date, 10);
    new_record->info.reg_date[10] = '\0';
    
    // 성적 정보 초기화
    new_record->subject_count = 0;
    new_record->gpa = 0.0;
    
    manager->count++;
    return 1;
}

// 학생 삭제 (학번 기준) - 성공 시 1, 실패 시 0 반환
int delete_student(StudentManager *manager, int id) {
    int index = -1;
    
    // 삭제할 학생 찾기
    for (int i = 0; i < manager->count; i++) {
        if (manager->records[i].info.id == id) {
            index = i;
            break;
        }
    }
    
    // 학생을 찾지 못한 경우
    if (index == -1) {
        fprintf(stderr, "오류: 학번 %d를 찾을 수 없습니다.\n", id);
        return 0;
    }
    
    // 삭제 후 배열 재정렬 (뒤의 요소들을 앞으로 이동)
    for (int i = index; i < manager->count - 1; i++) {
        manager->records[i] = manager->records[i + 1];
    }
    
    manager->count--;
    return 1;
}

// 학번으로 학생 검색 - 찾으면 해당 레코드 포인터, 못 찾으면 NULL 반환
StudentRecord* search_student_by_id(StudentManager *manager, int id) {
    for (int i = 0; i < manager->count; i++) {
        if (manager->records[i].info.id == id) {
            return &manager->records[i];
        }
    }
    return NULL;
}

// 이름으로 학생 검색 - 찾으면 해당 레코드 포인터, 못 찾으면 NULL 반환
// 주의: 동명이인이 있을 경우 첫 번째 학생만 반환
StudentRecord* search_student_by_name(StudentManager *manager, const char *name) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->records[i].info.name, name) == 0) {
            return &manager->records[i];
        }
    }
    return NULL;
}

// 학생 정보 수정 - 이름, 전공, 등록일 변경
void update_student_info(StudentRecord *record, const char *name, 
                        const char *major, const char *reg_date) {
    if (name) {
        strncpy(record->info.name, name, MAX_NAME_LEN - 1);
        record->info.name[MAX_NAME_LEN - 1] = '\0';
    }
    if (major) {
        strncpy(record->info.major, major, MAX_MAJOR_LEN - 1);
        record->info.major[MAX_MAJOR_LEN - 1] = '\0';
    }
    if (reg_date) {
        strncpy(record->info.reg_date, reg_date, 10);
        record->info.reg_date[10] = '\0';
    }
}

// 단일 학생 정보 출력
void display_student(const StudentRecord *record) {
    printf("\n==================== 학생 정보 ====================\n");
    printf("학번: %d\n", record->info.id);
    printf("이름: %s\n", record->info.name);
    printf("전공: %s\n", record->info.major);
    printf("등록일: %s\n", record->info.reg_date);
    printf("등록 과목 수: %d\n", record->subject_count);
    printf("평균 학점(GPA): %.2f\n", record->gpa);
    
    if (record->subject_count > 0) {
        printf("\n----- 과목별 성적 -----\n");
        for (int i = 0; i < record->subject_count; i++) {
            printf("  %d. %s: %.1f점 (%c학점, %d학점)\n",
                   i + 1,
                   record->grades[i].subject,
                   record->grades[i].score,
                   record->grades[i].grade,
                   record->grades[i].credit);
        }
    }
    printf("==================================================\n");
}

// 전체 학생 목록 출력 (간략 정보)
void display_all_students(const StudentManager *manager) {
    if (manager->count == 0) {
        printf("\n등록된 학생이 없습니다.\n");
        return;
    }
    
    printf("\n===================== 전체 학생 목록 =====================\n");
    printf("%-8s %-15s %-20s %-12s %s\n", 
           "학번", "이름", "전공", "등록일", "GPA(0~4.5)");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < manager->count; i++) {
        printf("%-8d %-15s %-20s %-12s %.2f\n",
               manager->records[i].info.id,
               manager->records[i].info.name,
               manager->records[i].info.major,
               manager->records[i].info.reg_date,
               manager->records[i].gpa);
    }
    
    printf("----------------------------------------------------------\n");
    printf("총 학생 수: %d명\n", manager->count);
    printf("==========================================================\n");
}
