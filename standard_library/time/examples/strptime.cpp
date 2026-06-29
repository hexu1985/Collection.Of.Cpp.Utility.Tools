#define _XOPEN_SOURCE      // 启用 strptime 声明（glibc 需要）
#include <stdio.h>
#include <time.h>

int main(void) {
    struct tm tm2;
    char *result;

    // ---------- 合法（格式完全匹配） ----------
    const char *valid = "2026-06-17,14-30-00";
    result = strptime(valid, "%Y-%m-%d,%H-%M-%S", &tm2);

    if (result != NULL && *result == '\0') {   // 成功且解析到字符串末尾
        printf("[合法] 解析成功：\n");
        printf("  年=%d, 月=%d, 日=%d, 时=%d, 分=%d, 秒=%d\n",
               tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday,
               tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
    } else {
        printf("[非法] 解析失败\n");
    }

    // ---------- 非法：格式错误（缺少逗号） ----------
    const char *invalid1 = "2026-06-17_14-30-00";   // 用下划线替代了逗号
    result = strptime(invalid1, "%Y-%m-%d,%H-%M-%S", &tm2);
    if (result != NULL && *result == '\0') {
        printf("[合法] 解析成功\n");
    } else {
        printf("[非法] 格式错误（缺少逗号），解析失败\n");
    }

    // ---------- 非法：月份值超出范围（但 strptime 不验证有效性） ----------
    const char *invalid2 = "2026-13-17,14-30-00";   // 月份为 13
    result = strptime(invalid2, "%Y-%m-%d,%H-%M-%S", &tm2);
    // 注意：strptime 通常不会因月份 >12 而返回 NULL，只是将 tm_mon 设为 12（或未定义）
    // 所以这里仅靠返回值无法检测这种“语义非法”的日期
    if (result != NULL && *result == '\0') {
        printf("[注意] 格式匹配，但日期语义可能无效（月份13）\n");
        // 若要严格验证，需额外检查 tm 字段范围
    } else {
        printf("[非法] 解析失败\n");
    }

    // ---------- 额外：成功但字符串尾部有残余字符 ----------
    const char *extra = "2026-06-17,14-30-00 extra";
    result = strptime(extra, "%Y-%m-%d,%H-%M-%S", &tm2);
    if (result != NULL && *result == '\0') {
        printf("[合法] 完全匹配\n");
    } else if (result != NULL) {
        printf("[部分匹配] 解析到 '%s'，剩余字符: '%s'\n", valid, result);
    } else {
        printf("[非法] 解析失败\n");
    }

    return 0;
}
