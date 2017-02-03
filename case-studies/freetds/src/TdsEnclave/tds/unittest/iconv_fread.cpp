/* FreeTDS - Library of routines accessing Sybase and Microsoft databases
 * Copyright (C) 2006   Frediano Ziglio
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "../../TdsEnclave.h"
#include "TdsEnclave_t.h"
#define INLINE_WRAP_STDIO_FUNC 1
#include "sgx_stdio_util.h"
#include "common.h"
#include "../../user_types.h"
#include <freetds/iconv.h>

#if HAVE_UNISTD_H
#undef getpid
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include <assert.h>

/* test tds_iconv_fread */

static char buf[4096+80];

int
ecall_unittest_iconv_fread_main(int argc, char **argv)
{
	static const char out_file[] = "iconv_fread.out";
	int i;
	SGX_WRAPPER_FILE f;
	TDSCONTEXT *ctx = tds_alloc_context(NULL);
	TDSSOCKET *tds = tds_alloc_socket(ctx, 512);
	TDSICONV * conv;
	const char *tdsdump;

	tdsdump = getenv("TDSDUMP");
	if (tdsdump)
		tdsdump_open(tdsdump);

	if (!ctx || !tds) {
		fprintf(stderr, "Error creating socket!\n");
		return 1;
	}

	tds_iconv_open(tds->conn, "ISO-8859-1", 0);

	conv = tds_iconv_get(tds->conn, "UTF-8", "ISO-8859-1");
	if (conv == NULL) {
		fprintf(stderr, "Error creating conversion, giving up!\n");
		return 1;
	}

	f = fopen(out_file, "w+b");
	if (!f) {
		fprintf(stderr, "Error opening file!\n");
		return 1;
	}

	for (i = 0; i < 4096+20; ++i) {
		char *out = NULL;
		size_t out_len = 0xdeadbeef;
		TDSRET res;
		const unsigned char x = 0x90;

		if (i == 32)
			i = 4096-20;

		/* write test string to SGX_WRAPPER_FILE */
		if (fseek(f, 0L, SEEK_SET)) {
			fprintf(stderr, "Error seeking!\n");
			return 1;
		}
		memset(buf, 'a', i);
		buf[i] = 0xC0 + (x >> 6);
		buf[i+1] = 0x80 + (x & 0x3f);
		buf[i+2] = '!';
		buf[i+3] = '!';

		fwrite(buf, 1, i+4, f);
		if (fseek(f, 0L, SEEK_SET)) {
			fprintf(stderr, "Error seeking!\n");
			return 1;
		}

		/* convert it */
		res = tds_bcp_fread(NULL, conv, f, "!!", 2, &out, &out_len);
		printf("res %d out_len %u\n", (int) res, (unsigned int) out_len);

		/* test */
		memset(buf, 'a', i);
		buf[i] = (char) x;
		assert(TDS_SUCCEED(res));
		if (out_len != i+1) {
			fprintf(stderr, "out %u bytes expected %d\n",
				(unsigned int) out_len, i+1);
			return 1;
		}
		assert(memcmp(out, buf, i+1) == 0);
		free(out);
	}
	fclose(f);
	// fclose(out_file);
	// unlink(out_file);

	tds_free_socket(tds);
	tds_free_context(ctx);
	return 0;
}