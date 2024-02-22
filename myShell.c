#include "myShell.h"
#include "myFunction.h"

int main()
{
    
    welcome();
    while (1)
    {
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        free(arguments);
        free(input);

    }
    return 1;
}

// יש לכתוב את פונקציית הברוכים הבאים כרצונכם אבל קצת יותר ממה שמוצג מטה לדוגמא:
//                     aSPY//YASa
//              apyyyyCY//////////YCa       |
//             sY//////YSpcs  scpCY//Pp     | Welcome to myShell
//  ayp ayyyyyyySCP//Pp           syY//C    | Version 2.4.3
//  AYAsAYYYYYYYY///Ps              cY//S   |
//          pCCCCY//p          cSSps y//Y   | https://github.com/<user>
//          SPPPP///a          pP///AC//Y   |
//               A//A            cyP////C   | Have fun!
//               p///Ac            sC///a   |
//               P////YCpc           A//A   | Craft packets like it is your last
//        scccccp///pSP///p          p//Y   | day on earth.
//       sY/////////y  caa           S//P   |                      -- Lao-Tze
//        cayCyayP//Ya              pY/Ya   |
//         sY/PsY////YCc          aC//Yp
//          sc  sccaCY//PCypaapyCP//YSs
//                   spCPY//////YPSps
//                        ccaacs
//                                        using c                                    using c
void welcome() {
    printf("\n");
    printf("*******************************************************\n");
    printf("*    AAAAA   dddd    iii   sssss    aaaa      sssss   *\n");
    printf("*   AA   AA  d   d    i   s        aa  aa    s        *\n");
    printf("*  AAAAAAAA  d   d    i    ssss   aaaaaaa    ssss     *\n");
    printf("*  AA    AA  d   d    i       s   aa   aa        s    *\n");
    printf("*  AA    AA  dddd    iii  ssss    aa   aa    ssss     *\n");
    printf("*******************************************************\n");
    printf("\n");
    printf("Welcome to myShell\n");
    printf("I Love Coding\n");
    printf("In the world of coding, every line is a thread in the fabric of functionality, weaving together the tapestry of technology.\n");
    printf("----Adi Sasportas----\n");
}