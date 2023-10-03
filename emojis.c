#include <stdio.h>
#include <Windows.h>

int main(void) {
    wchar_t emojis[] = L"😁😁😂🤣😃😄😅😆😉😊😋😎😍😘🥰😗😙😚☺🙂🤗🤩🤔🤨😐😑😶🙄😏😣😥😮🤐😯😪😫🥱😴😌"
                       "😛😜😝🤤😒😓😔😕🙃🤑😲☹🙁😖😞😟😤😢😭😦😧😨😩🤯😬😰😱🥵🥶😳🤪😵🥴😠😡🤬😷🤒🤕🤢🤮🤧😇🥳🥺"
                       "💉🥝🥥🍇🍈🍉🍊🍋🍌🍍🥭🍎🍏🍐🍑🍒🍓🍅🍆🌽🌶🍄🥑🥒🥬🥦🥔🧄🧅🥕🌰🥜💐🌸🏵🌹🌺🌻🌼🌷🥀☘🌱🌲🌳🌴"
                       "🌵🌾🌿🍀🍁🍂🍃❤🧡💛💚💙💜🤎🖤🤍💔❣💕💞💓💗💖💘💝💟💌💢💥💤💦💨💫🕳☮✝☪🕉☸✡🔯🕎☯☦";

    HANDLE hStdOut        = GetStdHandle(STD_OUTPUT_HANDLE);
    // WriteConsoleOutputW(hStdOut, emojis, sizeof emojis, );
    DWORD  dwBytesWritten = 0;
    WriteConsoleW(hStdOut, emojis, sizeof emojis, &dwBytesWritten, NULL);
    wprintf_s("%s\n", emojis);
    _putws(emojis);
    return 0;
}