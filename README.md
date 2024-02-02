# SimpleLexicalAnalYser
 Just another simple lexical analyser in C

This project is a simple implementation of a Lexical Analyzer in C. The Lexical Analyzer reads an input file character by character, identifies the lexemes in the input and categorizes them into corresponding token classes.

## Features

- Identifies Arithmetic Operators (ArOp)
- Identifies Relational Operators (RelOp)
- Identifies Assignment Operators (AssOp)
- Handles multi-line comments
- Handles invalid characters

## Code Snippet

The following code snippet is an example of how the Lexical Analyzer handles different cases:

```c
case 36:
    buffer[buf_idx] = '\0';
    printf("%s\n", buffer);
    printf("State 36\n");
    if (ch == '='){
        buffer[buf_idx++] = ch;
        fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
        state = 0;
    }
    else{
        printf("Invalid character %c\n", ch);
        fseek(fp, 1, SEEK_CUR);
    }
    break;
```

In the above code, the Lexical Analyzer is in state 36. If the current character is '=', it is identified as a Relational Operator. If the character is not '=', it is considered an invalid character and the file pointer is moved to the next character.

## Usage

To use this Lexical Analyzer, you need to have a C compiler installed on your system. You can compile the `LexicalAnalyser.c` file using the following command:

```bash
gcc LexicalAnalyser.c -o LexicalAnalyser
```

Then, you can run the program using the following command:

```bash
./LexicalAnalyser
```

Please note that the input file should be in the same directory as the `LexicalAnalyser.c` file.

## Contributing

Contributions are welcome. Please feel free to fork the project and create a pull request with your changes.

