It is a simple example of client-server application, written in C++ with Qt.
If run client with txt-file in command line arguments (example - SimpleTest.txt), it will connect to server and send text from the file. 
Server will analize text and return answer in Json-format, which contains: number of all words, number of unique words and the longes sequence of words without any duplicated words.
Both client and server are logging their steps in console.
Result of analisys for SimpleTest.txt is:
    Number of words: 20
    Number of unique words: 16
    The length of the longest sequence of unique words: 10
    The longest sequence of unique words: mean what I say that's the same thing you know
