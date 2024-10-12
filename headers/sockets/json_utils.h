#ifndef JSON_UTILS_H
#define JSON_UTILS_H

char* serialize_matrix(int matrix[10][10]);
int deserialize_matrix(const char *json_str, int matrix[10][10]);

#endif // JSON_UTILS_H
