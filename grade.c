#include "grade.h"

// 점수 유효성 검사 - 0.0 ~ 100.0 범위 확인
int is_valid_score(float score) {
    return (score >= 0.0 && score <= 100.0);
}

// 학점 유효성 검사 - 1 ~ 4 범위 확인
int is_valid_credit(int credit) {
    return (credit >= 1 && credit <= 4);
}

// 과목 중복 검사 - 이미 등록된 과목이면 1, 아니면 0 반환
int has_subject(const StudentRecord *record, const char *subject) {
    for (int i = 0; i < record->subject_count; i++) {
        if (strcmp(record->grades[i].subject, subject) == 0) {
            return 1;
        }
    }
    return 0;
}

// 점수를 등급으로 변환 (A, B, C, D, F)
char score_to_grade(float score) {
    if (score >= 90.0) return 'A';
    else if (score >= 80.0) return 'B';
    else if (score >= 70.0) return 'C';
    else if (score >= 60.0) return 'D';
    else return 'F';
}

// 과목 성적 추가 - 성공 시 1, 실패 시 0 반환
int add_grade(StudentRecord *record, const char *subject, 
              float score, int credit) {
    // 최대 과목 수 초과 검사
    if (record->subject_count >= MAX_SUBJECTS) {
        fprintf(stderr, "오류: 최대 과목 수(%d)를 초과했습니다.\n", MAX_SUBJECTS);
        return 0;
    }
    
    // 점수 유효성 검사
    if (!is_valid_score(score)) {
        fprintf(stderr, "오류: 유효하지 않은 점수입니다. (0.0 ~ 100.0)\n");
        return 0;
    }
    
    // 학점 유효성 검사
    if (!is_valid_credit(credit)) {
        fprintf(stderr, "오류: 유효하지 않은 학점입니다. (1 ~ 4)\n");
        return 0;
    }
    
    // 과목 중복 검사
    if (has_subject(record, subject)) {
        fprintf(stderr, "오류: 과목 '%s'은(는) 이미 등록되어 있습니다.\n", subject);
        return 0;
    }
    
    // 새 성적 추가
    Grade *new_grade = &record->grades[record->subject_count];
    strncpy(new_grade->subject, subject, MAX_NAME_LEN - 1);
    new_grade->subject[MAX_NAME_LEN - 1] = '\0';
    new_grade->score = score;
    new_grade->credit = credit;
    new_grade->grade = score_to_grade(score);
    
    record->subject_count++;
    
    // GPA 재계산
    record->gpa = calculate_gpa(record);
    
    return 1;
}

// 과목 성적 수정 - 성공 시 1, 실패 시 0 반환
int update_grade(StudentRecord *record, const char *subject, float new_score) {
    // 점수 유효성 검사
    if (!is_valid_score(new_score)) {
        fprintf(stderr, "오류: 유효하지 않은 점수입니다. (0.0 ~ 100.0)\n");
        return 0;
    }
    
    // 과목 찾기
    for (int i = 0; i < record->subject_count; i++) {
        if (strcmp(record->grades[i].subject, subject) == 0) {
            // 성적 수정
            record->grades[i].score = new_score;
            record->grades[i].grade = score_to_grade(new_score);
            
            // GPA 재계산
            record->gpa = calculate_gpa(record);
            
            return 1;
        }
    }
    
    // 과목을 찾지 못한 경우
    fprintf(stderr, "오류: 과목 '%s'을(를) 찾을 수 없습니다.\n", subject);
    return 0;
}

// 과목 성적 삭제 - 성공 시 1, 실패 시 0 반환
int delete_grade(StudentRecord *record, const char *subject) {
    int index = -1;
    
    // 삭제할 과목 찾기
    for (int i = 0; i < record->subject_count; i++) {
        if (strcmp(record->grades[i].subject, subject) == 0) {
            index = i;
            break;
        }
    }
    
    // 과목을 찾지 못한 경우
    if (index == -1) {
        fprintf(stderr, "오류: 과목 '%s'을(를) 찾을 수 없습니다.\n", subject);
        return 0;
    }
    
    // 삭제 후 배열 재정렬
    for (int i = index; i < record->subject_count - 1; i++) {
        record->grades[i] = record->grades[i + 1];
    }
    
    record->subject_count--;
    
    // GPA 재계산
    record->gpa = calculate_gpa(record);
    
    return 1;
}

// 특정 과목 검색 - 찾으면 해당 성적 포인터, 못 찾으면 NULL 반환
Grade* search_grade(StudentRecord *record, const char *subject) {
    for (int i = 0; i < record->subject_count; i++) {
        if (strcmp(record->grades[i].subject, subject) == 0) {
            return &record->grades[i];
        }
    }
    return NULL;
}

// 평균 점수 계산 (가중치 없는 단순 평균)
float calculate_average_score(const StudentRecord *record) {
    if (record->subject_count == 0) {
        return 0.0;
    }
    
    float total = 0.0;
    for (int i = 0; i < record->subject_count; i++) {
        total += record->grades[i].score;
    }
    
    return total / record->subject_count;
}

// 총 이수 학점 계산
float calculate_total_credits(const StudentRecord *record) {
    float total = 0.0;
    for (int i = 0; i < record->subject_count; i++) {
        total += record->grades[i].credit;
    }
    return total;
}

// GPA 계산 (학점 가중 평균)
float calculate_gpa(StudentRecord *record) {
    if (record->subject_count == 0) {
        return 0.0;
    }
    
    float weighted_sum = 0.0;
    float total_credits = 0.0;
    
    for (int i = 0; i < record->subject_count; i++) {
        weighted_sum += record->grades[i].score * record->grades[i].credit;
        total_credits += record->grades[i].credit;
    }
    
    if (total_credits == 0.0) {
        return 0.0;
    }
    
    // 0~100 범위의 GPA를 0~4.5 범위로 변환
    float gpa_100 = weighted_sum / total_credits;
    return gpa_100 * 0.045;  // ← 이 줄만 추가!
}

// 학생 성적표 출력 (상세 버전)
void display_transcript(const StudentRecord *record) {
    printf("\n===================== 성적표 =====================\n");
    printf("학번: %d\n", record->info.id);
    printf("이름: %s\n", record->info.name);
    printf("전공: %s\n", record->info.major);
    printf("==================================================\n\n");
    
    if (record->subject_count == 0) {
        printf("등록된 과목이 없습니다.\n");
        printf("==================================================\n");
        return;
    }
    
    printf("%-20s %8s %6s %6s\n", "과목명", "점수", "등급", "학점");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < record->subject_count; i++) {
        printf("%-20s %8.1f %6c %6d\n",
               record->grades[i].subject,
               record->grades[i].score,
               record->grades[i].grade,
               record->grades[i].credit);
    }
    
    printf("--------------------------------------------------\n");
    printf("총 이수 학점: %.0f학점\n", calculate_total_credits(record));
    printf("평균 점수(0~100): %.2f점\n", calculate_average_score(record));
    printf("학점 평균(GPA, 0~4.5): %.2f / 4.5\n", record->gpa);
    printf("==================================================\n");
}

// 성적 요약 정보 출력 (간략 버전)
void display_grade_summary(const StudentRecord *record) {
    printf("\n----- %s (%d) 성적 요약 -----\n", 
           record->info.name, record->info.id);
    
    if (record->subject_count == 0) {
        printf("등록된 과목이 없습니다.\n");
        return;
    }
    
    printf("등록 과목 수: %d개\n", record->subject_count);
    printf("총 이수 학점: %.0f학점\n", calculate_total_credits(record));
    printf("평균 점수(0~100): %.2f점\n", calculate_average_score(record));
    printf("GPA(0~4.5): %.2f / 4.5\n", record->gpa);
    
    // 등급별 과목 수 계산
    int grade_count[5] = {0}; // A, B, C, D, F
    for (int i = 0; i < record->subject_count; i++) {
        switch (record->grades[i].grade) {
            case 'A': grade_count[0]++; break;
            case 'B': grade_count[1]++; break;
            case 'C': grade_count[2]++; break;
            case 'D': grade_count[3]++; break;
            case 'F': grade_count[4]++; break;
        }
    }
    
    printf("등급 분포: A(%d) B(%d) C(%d) D(%d) F(%d)\n",
           grade_count[0], grade_count[1], grade_count[2],
           grade_count[3], grade_count[4]);
    printf("--------------------------------\n");
}
