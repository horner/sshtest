#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh/libssh.h>
#include <libssh/server.h>

int main(int argc, char **argv)
{
    ssh_bind sshbind;
    ssh_session sshsession;
    int rc;
    char buffer[256];
    const char *password = "password";

    // Create a new SSH bind object
    sshbind = ssh_bind_new();
    if (sshbind == NULL) {
        fprintf(stderr, "Error creating SSH bind object\n");
        exit(EXIT_FAILURE);
    }

    // Set options for the SSH bind object
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, "./ssh_host_rsa_key");
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT_STR, "2222");
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, "0.0.0.0");

    // Start listening for incoming SSH connections
    rc = ssh_bind_listen(sshbind);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error starting SSH listener: %s\n", ssh_get_error(sshbind));
        exit(EXIT_FAILURE);
    }

    // Accept an incoming SSH connection
    sshsession = ssh_new();
    if (sshsession == NULL) {
        fprintf(stderr, "Error creating new SSH session\n");
        exit(EXIT_FAILURE);
    }

    rc = ssh_bind_accept(sshbind, sshsession);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error accepting incoming SSH connection: %s\n", ssh_get_error(sshbind));
        exit(EXIT_FAILURE);
    }
printf("Accepted\n");
    rc = ssh_handle_key_exchange(sshsession);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error handling SSH key exchange: %s\n", ssh_get_error(sshsession));
        exit(EXIT_FAILURE);
    }
printf("Key exchange\n");
    ssh_set_auth_methods(sshsession, SSH_AUTH_METHOD_NONE );
    // Authenticate the SSH session using a password
    rc = ssh_userauth_password(sshsession, NULL, password);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating SSH session: %s\n", ssh_get_error(sshsession));
        exit(EXIT_FAILURE);
    }
printf("Authenticated\n");
    // Execute a shell command on the remote system
    ssh_channel channel = ssh_channel_new(sshsession);
    rc = ssh_channel_open_session(channel);
    rc = ssh_channel_request_exec(channel, "echo 'Hello, world!'");
    ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    printf("Remote output: %s\n", buffer);

    // Cleanup
    ssh_channel_free(channel);
    ssh_disconnect(sshsession);
    ssh_bind_free(sshbind);

    return 0;
}
