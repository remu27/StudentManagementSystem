#include "student.h"
#include "grade.h"
#include "utils.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 메뉴 출력 함수들
void print_main_menu(void);
void print_student_menu(void);
void print_grade_menu(void);
void print_search_menu(void);
void print_file_menu(void);

// 입력 처리 함수들
void clear_input_buffer(void);
int get_integer_input(const char *prompt);
float get_float_input(const char *prompt);
void get_string_input(const char *prompt, char *buffer, int size);
void press_enter_to_continue(void);

// 학생 관리 메뉴 함수들
void handle_student_menu(StudentManager *manager);
void add_student_interactive(StudentManager *manager);
void search_student_interactive(StudentManager *manager);
void delete_student_interactive(StudentManager *manager);
void update_student_interactive(StudentManager *manager);

// 성적 관리 메뉴 함수들
void handle_grade_menu(StudentManager *manager);
void add_grade_interactive(StudentManager *manager);
void update_grade_interactive(StudentManager *manager);
void delete_grade_interactive(StudentManager *manager);
void view_transcript_interactive(StudentManager *manager);

// 조회 및 분석 메뉴 함수들
void handle_search_menu(StudentManager *manager);
void view_ranking_interactive(StudentManager *manager);
void view_statistics_interactive(StudentManager *manager);
void view_subject_statistics_interactive(StudentManager *manager);
void search_by_gpa_range_interactive(StudentManager *manager);

// 파일 관리 메뉴 함수들
void handle_file_menu(StudentManager *manager);
void save_data_interactive(StudentManager *manager);
void load_data_interactive(StudentManager *manager);
void backup_data_interactive(StudentManager *manager);
void restore_data_interactive(StudentManager *manager);

// 입력 버퍼 비우기 (개행 문자 제거)
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 정수 입력 받기 (에러 처리 포함)
int get_integer_input(const char *prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        clear_input_buffer();
        printf("잘못된 입력입니다. 정수를 입력하세요.\n%s", prompt);
    }
    clear_input_buffer();
    return value;
}

// 실수 입력 받기 (에러 처리 포함)
float get_float_input(const char *prompt) {
    float value;
    printf("%s", prompt);
    while (scanf("%f", &value) != 1) {
        clear_input_buffer();
        printf("잘못된 입력입니다. 숫자를 입력하세요.\n%s", prompt);
    }
    clear_input_buffer();
    return value;
}

// 문자열 입력 받기
void get_string_input(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin)) {
        // 개행 문자 제거
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// 계속하려면 Enter 키 입력 대기
void press_enter_to_continue(void) {
    printf("\n계속하려면 Enter 키를 누르세요...");
    clear_input_buffer();
}

// 메인 메뉴 출력
void print_main_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("|     학생 성적 관리 시스템 v1.0       |\n");
    printf("========================================\n");
    printf("  1. 학생 관리\n");
    printf("  2. 성적 관리\n");
    printf("  3. 조회 및 분석\n");
    printf("  4. 파일 관리\n");
    printf("  5. 전체 학생 목록 보기\n");
    printf("  0. 종료\n");
    printf("────────────────────────────────────────\n");
}

// 학생 관리 메뉴 출력
void print_student_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("|          학생 관리 메뉴               |\n");
    printf("========================================\n");
    printf("  1. 학생 추가\n");
    printf("  2. 학생 검색\n");
    printf("  3. 학생 정보 수정\n");
    printf("  4. 학생 삭제\n");
    printf("  0. 메인 메뉴로 돌아가기\n");
    printf("────────────────────────────────────────\n");
}

// 성적 관리 메뉴 출력
void print_grade_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("|          성적 관리 메뉴               |\n");
    printf("========================================\n");
    printf("  1. 성적 입력\n");
    printf("  2. 성적 수정\n");
    printf("  3. 성적 삭제\n");
    printf("  4. 학생 성적표 조회\n");
    printf("  0. 메인 메뉴로 돌아가기\n");
    printf("────────────────────────────────────────\n");
}

// 조회 및 분석 메뉴 출력
void print_search_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("|        조회 및 분석 메뉴              |\n");
    printf("========================================\n");
    printf("  1. 성적 순위 조회\n");
    printf("  2. 전체 통계\n");
    printf("  3. 과목별 통계\n");
    printf("  4. GPA 범위로 검색\n");
    printf("  0. 메인 메뉴로 돌아가기\n");
    printf("────────────────────────────────────────\n");
}

// 파일 관리 메뉴 출력
void print_file_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("|          파일 관리 메뉴               |\n");
    printf("========================================\n");
    printf("  1. 데이터 저장\n");
    printf("  2. 데이터 불러오기\n");
    printf("  3. 백업 생성\n");
    printf("  4. 백업에서 복원\n");
    printf("  0. 메인 메뉴로 돌아가기\n");
    printf("────────────────────────────────────────\n");
}

// 학생 추가 (대화형)
void add_student_interactive(StudentManager *manager) {
    printf("\n=== 학생 추가 ===\n");
    
    int id = get_integer_input("학번: ");
    
    char name[MAX_NAME_LEN];
    get_string_input("이름: ", name, MAX_NAME_LEN);
    
    char major[MAX_MAJOR_LEN];
    get_string_input("전공: ", major, MAX_MAJOR_LEN);
    
    char reg_date[11];
    get_string_input("등록일 (YYYY-MM-DD): ", reg_date, 11);
    
    if (add_student(manager, id, name, major, reg_date)) {
        printf("\n? 학생이 추가되었습니다.\n");
    } else {
        printf("\n? 학생 추가에 실패했습니다.\n");
    }
}

// 학생 검색 (대화형)
void search_student_interactive(StudentManager *manager) {
    printf("\n=== 학생 검색 ===\n");
    printf("1. 학번으로 검색\n");
    printf("2. 이름으로 검색\n");
    
    int choice = get_integer_input("선택: ");
    StudentRecord *record = NULL;
    
    if (choice == 1) {
        int id = get_integer_input("학번: ");
        record = search_student_by_id(manager, id);
    } else if (choice == 2) {
        char name[MAX_NAME_LEN];
        get_string_input("이름: ", name, MAX_NAME_LEN);
        record = search_student_by_name(manager, name);
    }
    
    if (record) {
        display_student(record);
    } else {
        printf("\n학생을 찾을 수 없습니다.\n");
    }
}

// 학생 삭제 (대화형)
void delete_student_interactive(StudentManager *manager) {
    printf("\n=== 학생 삭제 ===\n");
    
    int id = get_integer_input("삭제할 학생의 학번: ");
    
    // 삭제 전 확인
    StudentRecord *record = search_student_by_id(manager, id);
    if (!record) {
        printf("\n학생을 찾을 수 없습니다.\n");
        return;
    }
    
    printf("\n삭제할 학생 정보:\n");
    printf("학번: %d\n", record->info.id);
    printf("이름: %s\n", record->info.name);
    printf("전공: %s\n", record->info.major);
    
    char confirm[10];
    get_string_input("\n정말 삭제하시겠습니까? (yes/no): ", confirm, 10);
    
    if (strcmp(confirm, "yes") == 0|| strcmp(confirm, "y") == 0) {
        if (delete_student(manager, id)) {
            printf("\n? 학생이 삭제되었습니다.\n");
        }
    } else {
        printf("\n취소되었습니다.\n");
    }
}

// 학생 정보 수정 (대화형)
void update_student_interactive(StudentManager *manager) {
    printf("\n=== 학생 정보 수정 ===\n");
    
    int id = get_integer_input("수정할 학생의 학번: ");
    StudentRecord *record = search_student_by_id(manager, id);
    
    if (!record) {
        printf("\n학생을 찾을 수 없습니다.\n");
        return;
    }
    
    printf("\n현재 정보:\n");
    display_student(record);
    
    printf("\n새 정보 입력 (변경하지 않으려면 Enter):\n");
    
    char name[MAX_NAME_LEN];
    char major[MAX_MAJOR_LEN];
    char reg_date[11];
    
    get_string_input("이름: ", name, MAX_NAME_LEN);
    get_string_input("전공: ", major, MAX_MAJOR_LEN);
    get_string_input("등록일: ", reg_date, 11);
    
    // 빈 문자열이 아닌 경우만 수정
    update_student_info(record,
                       strlen(name) > 0 ? name : NULL,
                       strlen(major) > 0 ? major : NULL,
                       strlen(reg_date) > 0 ? reg_date : NULL);
    
    printf("\n? 학생 정보가 수정되었습니다.\n");
}

// 성적 입력 (대화형)
void add_grade_interactive(StudentManager *manager) {
    printf("\n=== 성적 입력 ===\n");
    
    int id = get_integer_input("학번: ");
    StudentRecord *record = search_student_by_id(manager, id);
    
    if (!record) {
        printf("\n학생을 찾을 수 없습니다.\n");
        return;
    }
    
    printf("\n학생: %s (%d)\n", record->info.name, record->info.id);
    
    char subject[MAX_NAME_LEN];
    get_string_input("과목명: ", subject, MAX_NAME_LEN);
    
    float score = get_float_input("점수 (0~100): ");
    int credit = get_integer_input("학점 (1~4): ");
    
    if (add_grade(record, subject, score, credit)) {
        printf("\n? 성적이 입력되었습니다.\n");
        printf("현재 GPA: %.2f\n", record->gpa);
    }
}

// 성적 수정 (대화형)
void update_grade_interactive(StudentManager *manager) {
    printf("\n=== 성적 수정 ===\n");
    
    int id = get_integer_input("학번: ");
    StudentRecord *record = search_student_by_id(manager, id);
    
    if (!record) {
        printf("\n학생을 찾을 수 없습니다.\n");
        return;
    }
    
    if (record->subject_count == 0) {
        printf("\n등록된 성적이 없습니다.\n");
        return;
    }
    
    display_student(record);
    
    char subject[MAX_NAME_LEN];
    get_string_input("\n수정할 과목명: ", subject, MAX_NAME_LEN);
    
    float new_score = get_float_input("새 점수 (0~100): ");
    
    if (update_grade(record, subject, new_score)) {
        printf("\n? 성적이 수정되었습니다.\n");
        printf("현재 GPA: %.2f\n", record->gpa);
    }
}

// 성적 삭제 (대화형)
void delete_grade_interactive(StudentManager *manager) {
    printf("\n=== 성적 삭제 ===\n");
    
    int id = get_integer_input("학번: ");
    StudentRecord *record = search_student_by_id(manager, id);
    
    if (!record) {
        printf("\n학생을 찾을 수 없습니다.\n");
        return;
    }
    
    if (record->subject_count == 0) {
        printf("\n등록된 성적이 없습니다.\n");
        return;
    }
    
    display_student(record);
    
    char subject[MAX_NAME_LEN];
    get_string_input("\n삭제할 과목명: ", subject, MAX_NAME_LEN);
    
    if (delete_grade(record, subject)) {
        printf("\n? 성적이 삭제되었습니다.\n");
        printf("현재 GPA: %.2f\n", record->gpa);
    }
}

// 성적표 조회 (대화형)
void view_transcript_interactive(StudentManager *manager) {
    printf("\n=== 성적표 조회 ===\n");
    
    int id = get_integer_input("학번: ");
    StudentRecord *record = search_student_by_id(manager, id);
    
    if (record) {
        display_transcript(record);
    } else {
        printf("\n학생을 찾을 수 없습니다.\n");
    }
}

// 순위 조회 (대화형)
void view_ranking_interactive(StudentManager *manager) {
    if (manager->count == 0) {
        printf("\n등록된 학생이 없습니다.\n");
        return;
    }
    
    display_ranking_report(manager);
}

// 통계 조회 (대화형)
void view_statistics_interactive(StudentManager *manager) {
    if (manager->count == 0) {
        printf("\n등록된 학생이 없습니다.\n");
        return;
    }
    
    display_class_statistics(manager);
}

// 과목별 통계 (대화형)
void view_subject_statistics_interactive(StudentManager *manager) {
    char subject[MAX_NAME_LEN];
    get_string_input("\n과목명: ", subject, MAX_NAME_LEN);
    
    display_subject_statistics(manager, subject);
}

// GPA 범위 검색 (대화형)
void search_by_gpa_range_interactive(StudentManager *manager) {
    printf("\n=== GPA 범위 검색 (0.0 ~ 4.5) ===\n");
    
    float min_gpa = get_float_input("최소 GPA (0.0 ~ 4.5): ");
    float max_gpa = get_float_input("최대 GPA (0.0 ~ 4.5): ");
    
    // 입력값 범위 검증
    if (min_gpa < 0.0) min_gpa = 0.0;
    if (max_gpa > 4.5) max_gpa = 4.5;
    if (min_gpa > max_gpa) {
        printf("오류: 최소 GPA가 최대 GPA보다 작아야 합니다.\n");
        return;
    }
    
    search_students_by_gpa_range(manager, min_gpa, max_gpa);
}

// 데이터 저장 (대화형)
void save_data_interactive(StudentManager *manager) {
    printf("\n=== 데이터 저장 ===\n");
    
    if (save_data(manager, DEFAULT_DATA_FILE)) {
        printf("? 저장 완료\n");
    } else {
        printf("? 저장 실패\n");
    }
}

// 데이터 불러오기 (대화형)
void load_data_interactive(StudentManager *manager) {
    printf("\n=== 데이터 불러오기 ===\n");
    
    if (manager->count > 0) {
        char confirm[10];
        get_string_input("현재 데이터를 덮어씁니다. 계속하시겠습니까? (yes/no): ", 
                        confirm, 10);
        if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "y") != 0) {
            printf("취소되었습니다.\n");
            return;
        }
    }
    
    if (load_data(manager, DEFAULT_DATA_FILE)) {
        printf("? 불러오기 완료\n");
    } else {
        printf("? 불러오기 실패\n");
    }
}

// 백업 생성 (대화형)
void backup_data_interactive(StudentManager *manager) {
    printf("\n=== 백업 생성 ===\n");
    
    if (create_backup(DEFAULT_DATA_FILE)) {
        printf("? 백업 완료\n");
    } else {
        printf("? 백업 실패\n");
    }
}

// 백업 복원 (대화형)
void restore_data_interactive(StudentManager *manager) {
    printf("\n=== 백업에서 복원 ===\n");
    
    char confirm[10];
    get_string_input("현재 데이터를 덮어씁니다. 계속하시겠습니까? (yes/no): ", 
                    confirm, 10);
    if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "y") != 0) {
        printf("취소되었습니다.\n");
        return;
    }
    
    if (restore_from_backup(manager)) {
        printf("? 복원 완료\n");
    } else {
        printf("? 복원 실패\n");
    }
}

// 학생 관리 메뉴 처리
void handle_student_menu(StudentManager *manager) {
    int choice;
    
    do {
        print_student_menu();
        choice = get_integer_input("선택: ");
        
        switch (choice) {
            case 1:
                add_student_interactive(manager);
                break;
            case 2:
                search_student_interactive(manager);
                break;
            case 3:
                update_student_interactive(manager);
                break;
            case 4:
                delete_student_interactive(manager);
                break;
            case 0:
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
        
        if (choice != 0) press_enter_to_continue();
        
    } while (choice != 0);
}

// 성적 관리 메뉴 처리
void handle_grade_menu(StudentManager *manager) {
    int choice;
    
    do {
        print_grade_menu();
        choice = get_integer_input("선택: ");
        
        switch (choice) {
            case 1:
                add_grade_interactive(manager);
                break;
            case 2:
                update_grade_interactive(manager);
                break;
            case 3:
                delete_grade_interactive(manager);
                break;
            case 4:
                view_transcript_interactive(manager);
                break;
            case 0:
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
        
        if (choice != 0) press_enter_to_continue();
        
    } while (choice != 0);
}

// 조회 및 분석 메뉴 처리
void handle_search_menu(StudentManager *manager) {
    int choice;
    
    do {
        print_search_menu();
        choice = get_integer_input("선택: ");
        
        switch (choice) {
            case 1:
                view_ranking_interactive(manager);
                break;
            case 2:
                view_statistics_interactive(manager);
                break;
            case 3:
                view_subject_statistics_interactive(manager);
                break;
            case 4:
                search_by_gpa_range_interactive(manager);
                break;
            case 0:
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
        
        if (choice != 0) press_enter_to_continue();
        
    } while (choice != 0);
}

// 파일 관리 메뉴 처리
void handle_file_menu(StudentManager *manager) {
    int choice;
    
    do {
        print_file_menu();
        choice = get_integer_input("선택: ");
        
        switch (choice) {
            case 1:
                save_data_interactive(manager);
                break;
            case 2:
                load_data_interactive(manager);
                break;
            case 3:
                backup_data_interactive(manager);
                break;
            case 4:
                restore_data_interactive(manager);
                break;
            case 0:
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
        
        if (choice != 0) press_enter_to_continue();
        
    } while (choice != 0);
}

// 메인 함수
int main(void) {
    // StudentManager 생성 (최대 100명)
    StudentManager *manager = create_manager(MAX_STUDENTS);
    if (!manager) {
        fprintf(stderr, "시스템 초기화 실패\n");
        return 1;
    }
    
    // 기존 데이터 파일이 있으면 자동 로드
    if (file_exists(DEFAULT_DATA_FILE)) {
        printf("기존 데이터 파일을 발견했습니다.\n");
        load_data(manager, DEFAULT_DATA_FILE);
    }
    
    int choice;
    
    // 메인 루프
    do {
        print_main_menu();
        choice = get_integer_input("선택: ");
        
        switch (choice) {
            case 1:
                handle_student_menu(manager);
                break;
            case 2:
                handle_grade_menu(manager);
                break;
            case 3:
                handle_search_menu(manager);
                break;
            case 4:
                handle_file_menu(manager);
                break;
            case 5:
                display_all_students(manager);
                press_enter_to_continue();
                break;
            case 0:
                printf("\n프로그램을 종료합니다.\n");
                
                // 종료 전 저장 확인
                if (manager->count > 0) {
                    char save_choice[10];
                    get_string_input("변경사항을 저장하시겠습니까? (yes/no): ", 
                                    save_choice, 10);
                    if (strcmp(save_choice, "yes") == 0|| 
                        strcmp(save_choice, "y") == 0) {
                        save_data(manager, DEFAULT_DATA_FILE);
                    }
                }
                break;
            default:
                printf("잘못된 선택입니다.\n");
                press_enter_to_continue();
        }
        
    } while (choice != 0);
    
    // 메모리 해제
    destroy_manager(manager);
    
    printf("감사합니다!\n");
    return 0;
}
