#include "myFunction.h"

/*
הפונקציה strwok מתפקידת בעיקרה כדי לחלץ מילים ממחרוזת, 
כאשר המספר המוחזר של פעמים יכול להיות שונה מאוד. 
היא מקבלת שני פרמטרים: מחרוזת לחילוק 
(str) וסדרה של תווים לחילוק (delim).
 בכל קריאה לפונקציה, היא מחזירה את המילה הבאה מהמחרוזת.
הפונקציה משתמשת במשתנה סטטי 
(next_token) כדי לשמור על מיקום התחלת האסימון הבא במחרוזת בין קריאות לפונקציה.
 זה מאפשר לפונקציה להמשיך את החיפוש אחרי המילה הבאה בכל פעם שהיא נקראת.
 כאשר מועבר NULL למשתנה str, 
 הפונקציה ממשיכה מהמיקום ששמור ב־next_token.
הפונקציה מחפשת כל תו ב־delim ובודקת האם הוא נמצא במיקום הנוכחי במחרוזת. 
אם כן, היא מקדימה את האסימון הבא במחרוזת.
 אם האסימון הבא הוא תו ריק או הסוף של המחרוזת,
  היא מחזירה NULL כדי לסיים את הלולאה ולא לחזור על תהליך זה.
הפונקציה משתמשת בקוד שמתבצע בכל קריאה לפונקציה, כלומר בכל הופעה חדשה של הלולאה.
 היא מחזירה את המילה שנמצאת בין האסימון הנוכחי והאסימון הבא במחרוזת.
 */


char *strwok(char *str, const char *delim) {
    static char *next_token = NULL;
    if (str) next_token = str; // אתחול קריאה הראשונה
    if (!next_token || *next_token == '\0') return NULL; // End of string

    char *token_start = next_token; // התחלת האסימון הבא
    char *current; // הדמות הנוכחית נבדקת
    // דלג על תוחמים מובילים
    for (current = next_token; *current != '\0'; current++) {
        const char *d;
        for (d = delim; *d != '\0'; d++) {
            if (*current == *d) {
                
                token_start++;
                break;
            }
        }
        if (*d == '\0') break; // נמצא התחלה של אסימון
    }  
    if (*token_start == '\0') {
        next_token = token_start; // היכונו לשיחה הבאה להחזיר NULL
        return NULL;
    }
    next_token = token_start; //התחל לחפש את סוף האסימון
    for (current = token_start; *current != '\0'; current++) {
        const char *d;
        for (d = delim; *d != '\0'; d++) {
            if (*current == *d) {
                // נמצא את הסוף של האסימון
                *current = '\0'; // סיים את האסימון
                next_token = current + 1; // הגדר עבור האסימון הבא
                return token_start; // החזר את האסימון הנוכחי
            }
        }
    }

    // הגיע לקצה החוט
    next_token = current; // אין יותר אסימונים
    return token_start; // החזר את האסימון האחרון
}

// הפונקציה getInputFromUser מקבלת קלט מהמשתמש באופן תרדמתי ומחזירה מחרוזת עם הקלט.


char *getInputFromUser(){
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str + index) = '\0';

    return str;
}

/*
הפונקציה splitArgument מקבלת מחרוזת שכל רכיביה הם פרמטרים עבור פקודה כלשהי, 
ומפרקת את המחרוזת לתתי מחרוזות בהתאם לרווחים ביניהם.
 עבור פרמטרים שמוזגים בין גרשיים, היא מכינה מחרוזת כוללת שבה נפרדו על ידי רווחים,
  ומכניסה את המחרוזת הזו כפרמטר בפלט המתואר.
  הפונקציה מחזירה מערך דו-ממדי של מחרוזות, 
  שבו כל מחרוזת מייצגת ארגומנט נפרד עבור הפקודה.
  */
char **splitArgument(char *str)
{
    // str = cp file file file
    //[cp,file,file,file,NULL]
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strwok(str, " ");
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr;
    while ((subStr = strwok(NULL, " ")) != NULL)
    {
        size++;
        index++;
        *(argumnts + index) = subStr;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}
void getLocation() {
    char location[BUFF_SIZE];
    char hostname[BUFF_SIZE];
    char *username = getenv("USER"); // על מערכות דמויות יוניקס; השתמש ב-"USERNAME" עבור Windows

    if (getcwd(location, BUFF_SIZE) == NULL) {
        perror("Error retrieving current directory");
        return;
    }

    if (gethostname(hostname, BUFF_SIZE) != 0) {
        perror("Error retrieving hostname");
        return;
    }

    if (username == NULL) {
        fprintf(stderr, "Error retrieving username\n");
        return;
    }
   
    
    
    green() ;
    bold();
    printf("%s@%s:", username, hostname);
    reset();
    blue();
    printf("%s", location);
    reset();
    printf("$ ");
}
//פונקציה לדמות התנתקות
void logout(char *input) {
    free(input); 
    puts("Logout successful.");
    exit(EXIT_SUCCESS); 
}
void echo(char **arg) {
    while (*(++arg)) {             
        printf("%s ", *arg);
    }
    puts(""); 
}


 /*
הפונקציה cd מטפלת בפקודת העברת ספרייה 
(Change Directory) במערכת הקבצים. היא מקבלת מערך של מחרוזות (arg),
 כאשר המחרוזה הראשונה היא הפקודה עצמה 
 ("cd") והמחרוזות הבאות הן הנתיבים שבהם יש להעביר את הספרייה הנוכחית.
   */
// יש לטפל במקרים בהם מקבלים נתיב המכיל רווחים, תזכרו - נתיב כזה צריך להיות מסומן בגרשיים ולכן יש לוודא זאת ואם הוא אכן כזה
// שמכיל סוגריים אז יש לבנות מחרוזת חדשה שאותה יש לשלוח לפונקציה שמשנה נתיב לתהליך.
void cd(char **arg) {
    char path[1024] = {0}; //התאם את הגודל לפי הצורך

    if (arg[1] == NULL) {
        printf("-myShell: cd: missing operand\n");
        return;
    }

    //טיפול ישיר עבור "cd"
    if (strcmp(arg[1], "..") == 0 && arg[2] == NULL) {
        if (chdir("..") != 0) {
            perror("-myShell: cd: error changing to parent directory");
        }
        return;
    }

   //עבור שבילים במירכאות או בשבילים ללא רווחים
    if (arg[1][0] == '\"') {
       //נתיב מצוטט
        strncpy(path, arg[1] + 1, sizeof(path) - 1); // העתק ללא הציטוט המוביל
        size_t path_len = strlen(path);
        if (path[path_len - 1] == '\"') {
            path[path_len - 1] = '\0'; //הסר הצעת מחיר נגררת
        } else if (arg[2] != NULL) { // טפל במקרה שבו נתיב מפוצל בין ארגומנטים
            for (int i = 2; arg[i] != NULL; i++) {
                strcat(path, " ");
                (path, arg[i], sizeof(path) - strlen(path) - 1);
                if (arg[i][strlen(arg[i]) - 1] == '\"') {
                    path[strlen(path) - 1] = '\0'; //הסר ציטוט נגרר ושבור
                    break;
                }
            }
        }
    } else {
        // שביל לא "מצוטט" אבל אולי מכיל רווח
        strncpy(path, arg[1], sizeof(path) - 1);
        for (int i = 2; arg[i] != NULL; i++) {
            if (strlen(path) + strlen(arg[i]) + 2 > sizeof(path)) {
                printf("-myShell: cd: path is too long\n");
                return;
            }
            strcat(path, " ");
            strcat(path, arg[i]);
        }
    }

    //ניסיון לשנות ספרייה 
    if (chdir(path) != 0) {
        printf("-myShell: cd: %s: No such file or directory\n",path);
    }
}

 /*
הפונקציה cp מייצגת פונקציונליות של פקודת העתקה (copy)
 בשפת C. פונקציה זו מקבלת שני ארגומנטים: הראשון היא מחרוזת המייצגת את שם הקובץ המקורי שיש להעתיק,
  והשני היא מחרוזת המייצגת את
   שם הקובץ היעד שבו יש להעתיק את המידע.
    הפונקציה מאפשרת לנו להעתיק את התוכן של קובץ מקורי לקובץ יעד באמצעות קריאה וכתיבה ישירה לקבצים.
    */
void cp(char **arguments){
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("Erorr");
        fclose(src);
        return;
    }

    while((ch=fgetc(src))!=EOF){
        fputc(ch,des);
    }
    fclose(src);
    fclose(des);
}
void delete(char **path) {
       if (path == NULL || path[1] == NULL) {
        fprintf(stderr, "Error: Path is NULL or not provided.\n");
        return;
    }
    char fullPath[1024] = {0};
    for (int i = 1; path[i] != NULL; ++i) {
        if (i > 1) strcat(fullPath, " "); 
        strcat(fullPath, path[i]);
    }
    printf("Attempting to delete file: %s\n", fullPath);
    if (unlink(fullPath) == 0) {
        printf("File '%s' successfully deleted.\n", fullPath);
    } else {
        perror("Error deleting file");
 }
}

 /* 
הפונקציה systemCall שהצגת מקבלת מערך של מחרוזות (arg)
, שבו המחרוזה הראשונה היא שם של תוכנית או פקודה להפעלה
, והמחרוזות הבאות הם הארגומנטים שלה.

היא בודקת על ידי הפעלת fork האם היא פועלת בתוך תהליך הורה או תהליך ילד.
 אם fork נכשלה,
  היא מדפיסה הודעת שגיאה ומחזירה. אם היא פועלת בתוך תהליך ילד
 , היא מנסה להחליף את התהליך הנוכחי של הילד בתוכנית או פקודה 
 שהועברה אליה כארגומנט עם הארגומנט
 ים הנוספים שניתנו לפונקציה. אם execvp נכשלה, היא יוצאת מהתוכנית של הילד
  עם עזרת exit.

בסופו של דבר, אם הפונקציה הופעלה בהצלחה והילד הופק בהצלחה,
 היא תמשיך להפעיל את התוכנית שהופעלה בתהליך הילד.
  */ 
void systemCall(char **arg){

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
        }

    /*
הפונקציה mypipe מבצעת יצירת צינור 
(pipe) ויצירת תהליך ילד באמצעות קריאה ל־fork. 
הילד הראשון מבצע את הפקודה שנמצאת במערך argv1
, ומשנה את פלט הפקודה לצינור באמצעות הפונקציה dup. 
הילד השני מקבל כקלט את תוצאת הפקודה הראשונה מהצינור,
 ומבצע את הפקודה השנייה שנמצאת במערך argv2.

  */
void mypipe(char **argv1, char **argv2){

    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
           /* הרכיב הראשון של שורת הפקודה */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
           /* stdout עובר כעת לצינור */
             /* תהליך הילד אכן פקודה */
            execvp(argv1[0], argv1);
        }
        /* רכיב הפקודה השני של שורת הפקודה */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* קלט סטנדרטי מגיע כעת מצינור */
        execvp(argv2[0], argv2);
    }

 }
 void splitByDot(const char *input, char **part1, char **part2) {
    // Find the position of the first '.' in the input string
    const char *dot_pos = strchr(input, '.');
    if (dot_pos == NULL) {
        fprintf(stderr, "No '.' found in input string\n");
        *part1 = NULL;
        *part2 = NULL;
        return;
    }

    // Find the position of the first space after the first '.'
    const char *space_pos = strchr(dot_pos, ' ');
    if (space_pos == NULL) {
        fprintf(stderr, "No space found after '.' in input string\n");
        *part1 = NULL;
        *part2 = NULL;
        return;
    }

    // Calculate the lengths of the two parts
    size_t len1 = dot_pos - input;
    size_t len2 = strlen(space_pos);

    // Allocate memory for part1 and part2
    *part1 = (char *)malloc((len1 + 1) * sizeof(char)); // +1 for null terminator
    *part2 = (char *)malloc((len2 + 1) * sizeof(char)); // +1 for null terminator
    if (*part1 == NULL || *part2 == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Copy the two parts into their respective buffers
    strncpy(*part1, input, len1);
    (*part1)[len1] = '\0'; // Null-terminate part1
    strncpy(*part2, dot_pos, len2);
    (*part2)[len2] = '\0'; // Null-terminate part2
}



    void move(char **args)
{
    char srcPath[2048] = {0};
    char destPath[2048] = {0};

    strcat(srcPath, args[1]);
    if (args[2] != NULL)
    {
        strcat(srcPath, " ");
        strcat(srcPath, args[2]);
    }

    if (args[3] != NULL)
    {
        strcat(destPath, args[3]);
        if (args[4] != NULL)
        {
            strcat(destPath, " ");
            strcat(destPath, args[4]);
        }
    }

    printf("Attempting to move from '%s' to '%s'\n", srcPath, destPath);

    // Attempt to move/rename the file
    if (rename(srcPath, destPath) == 0)
    {
        printf("File successfully moved.\n");
    }
    else
    {
        perror("Error movingfile");
  }
 
}