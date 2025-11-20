#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main ()
{
    int in_fd, out_fd;
    int bytes;

    char buffer[5];

    // assume infile is already created
    in_fd = open("infile", O_RDWR);

    out_fd = open("outfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    bytes = read(in_fd, buffer, 5); // read 5 bytes into buffer
    printf("Bytes read: %d\n", bytes);

    bytes = write(out_fd, buffer, 5);
    printf("Bytes written: %d\n", bytes);

    close(in_fd);
    close(out_fd);

    return 0;
}
