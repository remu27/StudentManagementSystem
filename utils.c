#include "utils.h"

// 학번 기준 비교 함수 (오름차순)
int compare_by_id(const void *a, const void *b) {
    const StudentRecord *student_a = (const StudentRecord *)a;
    const StudentRecord *student_b = (const StudentRecord *)b;
    return student_a->info.id - student_b->info.id;
}

// 이름 기준 비교 함수 (사전순)
int compare_by_name(const void *a, const void *b) {
    const StudentRecord *student_a = (const StudentRecord *)a;
    const StudentRecord *student_b = (const StudentRecord *)b;
    return strcmp(student_a->info.name, student_b->info.name);
}

// GPA 기준 비교 함수 (내림차순 - 높은 점수가 앞에)
int compare_by_gpa(const void *a, const void *b) {
    const StudentRecord *student_a = (const StudentRecord *)a;
    const StudentRecord *student_b = (const StudentRecord *)b;
    
    // float 비교 시 주의: 단순 뺄셈 대신 조건문 사용
    if (student_a->gpa > student_b->gpa) return -1;
    if (student_a->gpa < student_b->gpa) return 1;
    return 0;
}

// 학번순 정렬 (오름차순)
void sort_by_id(StudentManager *manager) {
    if (manager->count <= 1) return;
    qsort(manager->records, manager->count, 
          sizeof(StudentRecord), compare_by_id);
}

// 이름순 정렬 (사전순)
void sort_by_name(StudentManager *manager) {
    if (manager->count <= 1) return;
    qsort(manager->records, manager->count, 
          sizeof(StudentRecord), compare_by_name);
}

// GPA순 정렬 (내림차순 - 높은 점수가 앞에)
void sort_by_gpa(StudentManager *manager) {
    if (manager->count <= 1) return;
    qsort(manager->records, manager->count, 
          sizeof(StudentRecord), compare_by_gpa);
}

// 전체 학생 순위 계산 - GPA 기준 내림차순
// 동점자는 같은 순위, 다음 순위는 건너뜀
void calculate_rankings(StudentManager *manager) {
    if (manager->count == 0) return;
    
    // 먼저 GPA로 정렬
    sort_by_gpa(manager);
    
    printf("\n=============== 학생 순위 (GPA 기준) ===============\n");
    printf("%-6s %-8s %-15s %-20s %s\n", 
           "순위", "학번", "이름", "전공", "GPA(0~4.5)");
    printf("----------------------------------------------------\n");
    
    int rank = 1;
    for (int i = 0; i < manager->count; i++) {
        // 동점자 처리: 이전 학생과 GPA가 같으면 같은 순위
        if (i > 0 && manager->records[i].gpa < manager->records[i-1].gpa) {
            rank = i + 1;
        }
        
        printf("%-6d %-8d %-15s %-20s %.2f\n",
               rank,
               manager->records[i].info.id,
               manager->records[i].info.name,
               manager->records[i].info.major,
               manager->records[i].gpa);
    }
    printf("====================================================\n");
}

// 전체 학생 평균 GPA 계산
float get_class_average_gpa(const StudentManager *manager) {
    if (manager->count == 0) return 0.0;
    
    float total_gpa = 0.0;
    int valid_count = 0;
    
    for (int i = 0; i < manager->count; i++) {
        // 과목이 등록된 학생만 포함
        if (manager->records[i].subject_count > 0) {
            total_gpa += manager->records[i].gpa;
            valid_count++;
        }
    }
    
    return (valid_count > 0) ? (total_gpa / valid_count) : 0.0;
}

// 최고 GPA 찾기
float get_highest_gpa(const StudentManager *manager) {
    if (manager->count == 0) return 0.0;
    
    float max_gpa = manager->records[0].gpa;
    for (int i = 1; i < manager->count; i++) {
        if (manager->records[i].gpa > max_gpa) {
            max_gpa = manager->records[i].gpa;
        }
    }
    return max_gpa;
}

// 최저 GPA 찾기
float get_lowest_gpa(const StudentManager *manager) {
    if (manager->count == 0) return 0.0;
    
    float min_gpa = manager->records[0].gpa;
    for (int i = 1; i < manager->count; i++) {
        // 과목이 없는 학생은 제외
        if (manager->records[i].subject_count > 0 && 
            manager->records[i].gpa < min_gpa) {
            min_gpa = manager->records[i].gpa;
        }
    }
    return min_gpa;
}

// 1등 학생 찾기 (최고 GPA)
StudentRecord* get_top_student(StudentManager *manager) {
    if (manager->count == 0) return NULL;
    
    StudentRecord *top = &manager->records[0];
    for (int i = 1; i < manager->count; i++) {
        if (manager->records[i].gpa > top->gpa) {
            top = &manager->records[i];
        }
    }
    return top;
}

// 특정 과목의 전체 평균 점수 계산
float get_subject_average(const StudentManager *manager, const char *subject) {
    float total_score = 0.0;
    int count = 0;
    
    for (int i = 0; i < manager->count; i++) {
        for (int j = 0; j < manager->records[i].subject_count; j++) {
            if (strcmp(manager->records[i].grades[j].subject, subject) == 0) {
                total_score += manager->records[i].grades[j].score;
                count++;
                break;
            }
        }
    }
    
    return (count > 0) ? (total_score / count) : 0.0;
}

// 특정 과목을 수강하는 학생 수 계산
int count_students_with_subject(const StudentManager *manager, const char *subject) {
    int count = 0;
    
    for (int i = 0; i < manager->count; i++) {
        for (int j = 0; j < manager->records[i].subject_count; j++) {
            if (strcmp(manager->records[i].grades[j].subject, subject) == 0) {
                count++;
                break;
            }
        }
    }
    
    return count;
}

// 특정 과목의 통계 출력 (평균, 최고점, 최저점, 수강 인원)
void display_subject_statistics(const StudentManager *manager, const char *subject) {
    int count = 0;
    float total = 0.0;
    float max_score = 0.0;
    float min_score = 100.0;
    int grade_count[5] = {0}; // A, B, C, D, F
    
    printf("\n========== 과목 통계: %s ==========\n", subject);
    
    // 데이터 수집
    for (int i = 0; i < manager->count; i++) {
        for (int j = 0; j < manager->records[i].subject_count; j++) {
            if (strcmp(manager->records[i].grades[j].subject, subject) == 0) {
                float score = manager->records[i].grades[j].score;
                char grade = manager->records[i].grades[j].grade;
                
                total += score;
                count++;
                
                if (score > max_score) max_score = score;
                if (score < min_score) min_score = score;
                
                // 등급별 카운트
                switch (grade) {
                    case 'A': grade_count[0]++; break;
                    case 'B': grade_count[1]++; break;
                    case 'C': grade_count[2]++; break;
                    case 'D': grade_count[3]++; break;
                    case 'F': grade_count[4]++; break;
                }
                break;
            }
        }
    }
    
    // 통계 출력
    if (count == 0) {
        printf("수강하는 학생이 없습니다.\n");
    } else {
        printf("수강 인원: %d명\n", count);
        printf("평균 점수: %.2f점\n", total / count);
        printf("최고 점수: %.1f점\n", max_score);
        printf("최저 점수: %.1f점\n", min_score);
        printf("등급 분포: A(%d) B(%d) C(%d) D(%d) F(%d)\n",
               grade_count[0], grade_count[1], grade_count[2],
               grade_count[3], grade_count[4]);
    }
    printf("==========================================\n");
}

// GPA 범위로 학생 검색 (min_gpa ≤ GPA ≤ max_gpa)
void search_students_by_gpa_range(const StudentManager *manager, 
                                  float min_gpa, float max_gpa) {
    int found = 0;
    
    printf("\n====== GPA 범위 검색 (0.0 ~ 4.5): %.2f ~ %.2f ======\n", min_gpa, max_gpa);
    printf("%-8s %-15s %-20s %s\n", "학번", "이름", "전공", "GPA");
    printf("-----------------------------------------------\n");
    
    for (int i = 0; i < manager->count; i++) {
        if (manager->records[i].gpa >= min_gpa && 
            manager->records[i].gpa <= max_gpa) {
            printf("%-8d %-15s %-20s %.2f\n",
                   manager->records[i].info.id,
                   manager->records[i].info.name,
                   manager->records[i].info.major,
                   manager->records[i].gpa);
            found++;
        }
    }
    
    if (found == 0) {
        printf("해당 범위의 학생이 없습니다.\n");
    } else {
        printf("-----------------------------------------------\n");
        printf("총 %d명 검색됨\n", found);
    }
    printf("===============================================\n");
}

// 특정 등급을 받은 과목이 있는 학생 검색
void search_students_by_grade(const StudentManager *manager, char grade) {
    int found = 0;
    
    printf("\n====== 등급 검색: %c학점 이상 받은 학생 ======\n", grade);
    printf("%-8s %-15s %-20s %s\n", "학번", "이름", "과목명", "점수");
    printf("-----------------------------------------------\n");
    
    for (int i = 0; i < manager->count; i++) {
        for (int j = 0; j < manager->records[i].subject_count; j++) {
            if (manager->records[i].grades[j].grade == grade) {
                printf("%-8d %-15s %-20s %.1f\n",
                       manager->records[i].info.id,
                       manager->records[i].info.name,
                       manager->records[i].grades[j].subject,
                       manager->records[i].grades[j].score);
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("해당 등급을 받은 학생이 없습니다.\n");
    }
    printf("===============================================\n");
}

// 순위 리포트 출력 (상위 N명)
void display_ranking_report(const StudentManager *manager) {
    if (manager->count == 0) {
        printf("\n등록된 학생이 없습니다.\n");
        return;
    }
    
    // GPA로 정렬 (원본 유지를 위해 복사본 생성은 하지 않음)
    calculate_rankings(manager);
}

// 전체 반 통계 출력
void display_class_statistics(const StudentManager *manager) {
    if (manager->count == 0) {
        printf("\n등록된 학생이 없습니다.\n");
        return;
    }
    
    printf("\n=============== 반 전체 통계 ===============\n");
    printf("총 학생 수: %d명\n", manager->count);
    
    // 과목이 등록된 학생 수 계산
    int students_with_grades = 0;
    for (int i = 0; i < manager->count; i++) {
        if (manager->records[i].subject_count > 0) {
            students_with_grades++;
        }
    }
    
    printf("성적이 등록된 학생 수: %d명\n", students_with_grades);
    
    if (students_with_grades > 0) {
        printf("\n----- GPA 통계 -----\n");
        printf("평균 GPA(0~4.5): %.2f / 4.5\n", get_class_average_gpa(manager));
        printf("최고 GPA(0~4.5): %.2f / 4.5\n", get_highest_gpa(manager));
        printf("최저 GPA(0~4.5): %.2f / 4.5\n", get_lowest_gpa(manager));
        
        // 1등 학생 정보
        StudentRecord *top = get_top_student(manager);
        if (top) {
            printf("\n----- 1등 학생 -----\n");
            printf("학번: %d\n", top->info.id);
            printf("이름: %s\n", top->info.name);
            printf("전공: %s\n", top->info.major);
            printf("GPA: %.2f\n", top->gpa);
        }
        
        // GPA 구간별 학생 수 (0~4.5 범위)
        int gpa_ranges[5] = {0}; // 4.0+, 3.5+, 3.0+, 2.5+, 2.5-
        for (int i = 0; i < manager->count; i++) {
            if (manager->records[i].subject_count > 0) {
                float gpa = manager->records[i].gpa;
                if (gpa >= 4.0) gpa_ranges[0]++;
                else if (gpa >= 3.5) gpa_ranges[1]++;
                else if (gpa >= 3.0) gpa_ranges[2]++;
                else if (gpa >= 2.5) gpa_ranges[3]++;
                else gpa_ranges[4]++;
            }
        }

        printf("\n----- GPA 분포 -----\n");
        printf("4.0점 이상: %d명\n", gpa_ranges[0]);
        printf("3.5~3.99점: %d명\n", gpa_ranges[1]);
        printf("3.0~3.49점: %d명\n", gpa_ranges[2]);
        printf("2.5~2.99점: %d명\n", gpa_ranges[3]);
        printf("2.5점 미만: %d명\n", gpa_ranges[4]);

    }
    
    printf("============================================\n");
}
