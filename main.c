/*
 *
 * Copyright 2023 Kenichi Yasukata
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>

#include <dlfcn.h>

#include <libelisa.h>

int main(int argc, char *const *argv)
{
	int ch;
	int server_port = -1;
	const char *server_addr = NULL;
	const char *filename = NULL;

	while ((ch = getopt(argc, argv, "f:p:s:")) != -1) {
		switch (ch) {
		case 'f':
			filename = optarg;
			break;
		case 'p':
			server_port = atoi(optarg);
			break;
		case 's':
			server_addr = optarg;
			break;
		default:
			assert(0);
			break;
		}
	}

	assert(filename);

	{
		void *handle;
		assert((handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL)) != NULL);
		{
			int (*exec_init)(void);
			assert((exec_init = dlsym(handle, "elisa__exec_init")) != NULL);
			assert(!exec_init());

		}
		if (!server_addr) {
			void *server_cb, *server_exit_cb;
			assert((server_cb = dlsym(handle, "elisa__server_cb")) != NULL);
			assert((server_exit_cb = dlsym(handle, "elisa__server_exit_cb")) != NULL);
			elisa_server(server_port, server_cb, server_exit_cb);
		} else {
			void *client_cb;
			assert((client_cb = dlsym(handle, "elisa__client_cb")) != NULL);
			{
				int fd = elisa_client(server_addr, server_port, client_cb);
				{
					int (*client_work)(void);
					assert((client_work = dlsym(handle, "elisa__client_work")) != NULL);
					assert(!client_work());
				}
				close(fd);
			}
		}
	}

	return 0;
}
