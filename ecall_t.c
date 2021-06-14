/* EDL definition.
enclave {
  struct ShallowStruct {
    uint64_t count;
    size_t size;
    uint64_t* ptr;
  };

  struct CountStruct {
    uint64_t count;
    size_t size;
    [count=3] uint64_t* ptr;
  };

  trusted {
    public void deepcopy_nested_out([out, count=1] NestedStruct* n);
  };

  untrusted {
    void ocall_deepcopy_nested_out([out, count=1] NestedStruct* n);
  };
 */

/* Example of ECALL implementation.
void deepcopy_nested_out(NestedStruct* n)
{
    n->plain_int = 100;
    n->array_of_int = (int*)malloc(4 * sizeof(int));
    for (int i = 0; i < 4; i++)
        n->array_of_int[i] = i;
    n->shallow_struct = NULL;
    n->array_of_struct = (CountStruct*)malloc(3 * sizeof(CountSizeStruct));
    for (int i = 0; i < 3; i++)
    {
        n->array_of_struct[i].ptr = (uint64_t*)malloc(3 * sizeof(uint64_t));
        n->array_of_struct[i].count = 100;
        n->array_of_struct[i].size = 200;
        for (int j = 0; j < 3; j++)
            n->array_of_struct[i].ptr[j] = data[j];
    }
}
 */

/* Generated structs */
typedef struct ShallowStruct
{
    uint64_t count;
    size_t size;
    uint64_t* ptr;
} ShallowStruct;

typedef struct CountStruct
{
    uint64_t count;
    size_t size;
    uint64_t* ptr;
} CountStruct;

typedef struct NestedStruct
{
    int plain_int;
    int* array_of_int;
    ShallowStruct* shallow_struct;
    CountStruct* array_of_struct;
} NestedStruct;

/* EDL ECALL stub */
static void ecall_deepcopy_nested_out(
    uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    oe_result_t _result = OE_FAILURE;

    /* Prepare parameters. */
    deepcopy_nested_out_args_t* pargs_in =
        (deepcopy_nested_out_args_t*)input_buffer;
    deepcopy_nested_out_args_t* pargs_out =
        (deepcopy_nested_out_args_t*)output_buffer;

    size_t input_buffer_offset = 0;
    size_t output_buffer_offset = 0;
    OE_ADD_SIZE(input_buffer_offset, sizeof(*pargs_in));
    OE_ADD_SIZE(output_buffer_offset, sizeof(*pargs_out));

    /* Make sure input and output buffers lie within the enclave. */
    /* oe_is_within_enclave explicitly checks if buffers are null or not. */
    if (!oe_is_within_enclave(input_buffer, input_buffer_size))
        goto done;

    if (!oe_is_within_enclave(output_buffer, output_buffer_size))
        goto done;

    /* Set in and in-out pointers. */
    /* There were no in nor in-out parameters. */

    /* Set out and in-out pointers. */
    /* In-out parameters are copied to output buffer. */
    if (pargs_in->n)
        OE_SET_OUT_POINTER(
            n, ((size_t)1 * sizeof(NestedStruct)), NestedStruct*);

    /* Check that in/in-out strings are null terminated. */
    /* There were no in nor in-out string parameters. */

    /* lfence after checks. */
    oe_lfence();

    /* Call user function. */
    deepcopy_nested_out(pargs_in->n);

    if (pargs_out->n)
    {
        NestedStruct* _rhs = pargs_in->n;
        for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
        {
            pargs_out->n[_i_1].plain_int = _rhs[_i_1].plain_int;
            pargs_out->n[_i_1].shallow_struct = _rhs[_i_1].shallow_struct;
            pargs_out->n[_i_1].array_of_int = NULL;
            if (_rhs[_i_1].array_of_int)
            {
                int* _l_1_array_of_int =
                    (int*)oe_host_malloc(((size_t)4 * sizeof(int)));
                pargs_out->n[_i_1].array_of_int = _l_1_array_of_int;
                if (!_l_1_array_of_int)
                {
                    _result = OE_FAILURE;
                    goto done;
                }
                memcpy(
                    _l_1_array_of_int,
                    _rhs[_i_1].array_of_int,
                    ((size_t)4 * sizeof(int)));
            }
            CountStruct* _l_1_array_of_struct = NULL;
            if (_rhs[_i_1].array_of_struct)
            {
                _l_1_array_of_struct = (CountStruct*)oe_host_malloc(
                    ((size_t)3 * sizeof(CountStruct)));
                pargs_out->n[_i_1].array_of_struct = _l_1_array_of_struct;
                if (!_l_1_array_of_struct)
                {
                    _result = OE_FAILURE;
                    goto done;
                }
                for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                {
                    _l_1_array_of_struct[_i_2].count =
                        _rhs[_i_1].array_of_struct[_i_2].count;
                    _l_1_array_of_struct[_i_2].size =
                        _rhs[_i_1].array_of_struct[_i_2].size;
                    _l_1_array_of_struct[_i_2].ptr = NULL;
                    if (_rhs[_i_1].array_of_struct[_i_2].ptr)
                    {
                        uint64_t* _l_2_ptr = (uint64_t*)oe_host_malloc(
                            ((size_t)3 * sizeof(uint64_t)));
                        _l_1_array_of_struct[_i_2].ptr = _l_2_ptr;
                        if (!_l_2_ptr)
                        {
                            _result = OE_FAILURE;
                            goto done;
                        }
                        memcpy(
                            _l_2_ptr,
                            _rhs[_i_1].array_of_struct[_i_2].ptr,
                            ((size_t)3 * sizeof(uint64_t)));
                    }
                }
            }
        }
    }
    /* Success. */
    _result = OE_OK;
    *output_bytes_written = output_buffer_offset;

done:
    /* Free nested host buffers on failure. */
    if (_result != OE_OK)
    {
        if (pargs_out->n)
        {
            for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
            {
                oe_host_free(pargs_out->n[_i_1].array_of_int);
                pargs_out->n[_i_1].array_of_int = NULL;
                if (pargs_out->n[_i_1].array_of_struct)
                {
                    for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                    {
                        oe_host_free(
                            pargs_out->n[_i_1].array_of_struct[_i_2].ptr);
                        pargs_out->n[_i_1].array_of_struct[_i_2].ptr = NULL;
                    }
                }
                oe_host_free(pargs_out->n[_i_1].array_of_struct);
                pargs_out->n[_i_1].array_of_struct = NULL;
            }
        }
    }
    /* Free nested buffers allocated by the enclave. */
    if (pargs_in->n)
    {
        for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
        {
            oe_free(pargs_in->n[_i_1].array_of_int);
            pargs_in->n[_i_1].array_of_int = NULL;
            if (pargs_in->n[_i_1].array_of_struct)
            {
                for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                {
                    oe_free(pargs_in->n[_i_1].array_of_struct[_i_2].ptr);
                    pargs_in->n[_i_1].array_of_struct[_i_2].ptr = NULL;
                }
            }
            oe_free(pargs_in->n[_i_1].array_of_struct);
            pargs_in->n[_i_1].array_of_struct = NULL;
        }
    }

    if (output_buffer_size >= sizeof(*pargs_out) &&
        oe_is_within_enclave(pargs_out, output_buffer_size))
        pargs_out->_result = _result;
}

/* EDL OCALL wrapper */
oe_result_t ocall_deepcopy_nested_out(NestedStruct* n)
{
    oe_result_t _result = OE_FAILURE;

    /* If the enclave is in crashing/crashed status, new OCALL should fail
       immediately. */
    if (oe_get_enclave_status() != OE_OK)
        return oe_get_enclave_status();

    /* Marshalling struct. */
    ocall_deepcopy_nested_out_args_t _args, *_pargs_in = NULL,
                                            *_pargs_out = NULL;
    /* Marshalling buffer and sizes. */
    size_t _input_buffer_size = 0;
    size_t _output_buffer_size = 0;
    size_t _total_buffer_size = 0;
    uint8_t* _buffer = NULL;
    uint8_t* _input_buffer = NULL;
    uint8_t* _output_buffer = NULL;
    size_t _input_buffer_offset = 0;
    size_t _output_buffer_offset = 0;
    size_t _output_bytes_written = 0;

    /* Fill marshalling struct. */
    memset(&_args, 0, sizeof(_args));
    _args.n = (NestedStruct*)n;

    /* Compute input buffer size. Include in and in-out parameters. */
    OE_ADD_SIZE(_input_buffer_size, sizeof(ocall_deepcopy_nested_out_args_t));
    /* There were no corresponding parameters. */

    /* Compute output buffer size. Include out and in-out parameters. */
    OE_ADD_SIZE(_output_buffer_size, sizeof(ocall_deepcopy_nested_out_args_t));
    if (n)
        OE_ADD_SIZE(_output_buffer_size, ((size_t)1 * sizeof(NestedStruct)));

    /* Allocate marshalling buffer. */
    _total_buffer_size = _input_buffer_size;
    OE_ADD_SIZE(_total_buffer_size, _output_buffer_size);
    _buffer = (uint8_t*)oe_allocate_ocall_buffer(_total_buffer_size);
    _input_buffer = _buffer;
    _output_buffer = _buffer + _input_buffer_size;
    if (_buffer == NULL)
    {
        _result = OE_OUT_OF_MEMORY;
        goto done;
    }

    /* Serialize buffer inputs (in and in-out parameters). */
    _pargs_in = (ocall_deepcopy_nested_out_args_t*)_input_buffer;
    OE_ADD_SIZE(_input_buffer_offset, sizeof(*_pargs_in));
    /* There were no in nor in-out parameters. */

    /* Copy args structure (now filled) to input buffer. */
    memcpy(_pargs_in, &_args, sizeof(*_pargs_in));

    /* Call host function. */
    if ((_result = oe_call_host_function(
             all_fcn_id_ocall_deepcopy_nested_out,
             _input_buffer,
             _input_buffer_size,
             _output_buffer,
             _output_buffer_size,
             &_output_bytes_written)) != OE_OK)
        goto done;

    /* Setup output arg struct pointer. */
    _pargs_out = (ocall_deepcopy_nested_out_args_t*)_output_buffer;
    OE_ADD_SIZE(_output_buffer_offset, sizeof(*_pargs_out));

    /* Check if the call succeeded. */
    if ((_result = _pargs_out->_result) != OE_OK)
        goto done;

    /* Currently exactly _output_buffer_size bytes must be written. */
    if (_output_bytes_written != _output_buffer_size)
    {
        _result = OE_FAILURE;
        goto done;
    }

    /* Initialize the deep-copyable, out-only parameters pointed by _pargs_out
       to NULL.
       These parameters will be freed at the end of the function if not NULL. */
    _pargs_out->n = NULL;

    /* Unmarshal return value and out, in-out parameters. */
    /* No return value. */

    if (n)
    {
        _pargs_out->n = (NestedStruct*)(_output_buffer + _output_buffer_offset);
        OE_ADD_SIZE(
            _output_buffer_offset, (size_t)((size_t)1 * sizeof(NestedStruct)));
        for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
        {
            n[_i_1].plain_int = _pargs_out->n[_i_1].plain_int;
            n[_i_1].shallow_struct = _pargs_out->n[_i_1].shallow_struct;
            n[_i_1].array_of_int = NULL;
            if (_pargs_out->n[_i_1].array_of_int)
            {
                n[_i_1].array_of_int =
                    (int*)oe_malloc(((size_t)4 * sizeof(int)));
                if (!n[_i_1].array_of_int)
                {
                    _result = OE_FAILURE;
                    goto done;
                }
                memcpy(
                    n[_i_1].array_of_int,
                    _pargs_out->n[_i_1].array_of_int,
                    ((size_t)4 * sizeof(int)));
            }
            n[_i_1].array_of_struct = NULL;
            if (_pargs_out->n[_i_1].array_of_struct)
            {
                n[_i_1].array_of_struct =
                    (CountStruct*)oe_malloc(((size_t)3 * sizeof(CountStruct)));
                if (!n[_i_1].array_of_struct)
                {
                    _result = OE_FAILURE;
                    goto done;
                }
                for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                {
                    n[_i_1].array_of_struct[_i_2].count =
                        _pargs_out->n[_i_1].array_of_struct[_i_2].count;
                    n[_i_1].array_of_struct[_i_2].size =
                        _pargs_out->n[_i_1].array_of_struct[_i_2].size;
                    n[_i_1].array_of_struct[_i_2].ptr = NULL;
                    if (_pargs_out->n[_i_1].array_of_struct[_i_2].ptr)
                    {
                        n[_i_1].array_of_struct[_i_2].ptr =
                            (uint64_t*)oe_malloc(
                                ((size_t)3 * sizeof(uint64_t)));
                        if (!n[_i_1].array_of_struct[_i_2].ptr)
                        {
                            _result = OE_FAILURE;
                            goto done;
                        }
                        memcpy(
                            n[_i_1].array_of_struct[_i_2].ptr,
                            _pargs_out->n[_i_1].array_of_struct[_i_2].ptr,
                            ((size_t)3 * sizeof(uint64_t)));
                    }
                }
            }
        }
    }

    /* Retrieve propagated errno from OCALL. */
    /* Errno propagation not enabled. */

    _result = OE_OK;

done:
    if (_buffer)
        oe_free_ocall_buffer(_buffer);

    /* Free nested enclave buffers on failure. */
    if (_result != OE_OK)
    {
        if (n)
        {
            for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
            {
                oe_free(n[_i_1].array_of_int);
                n[_i_1].array_of_int = NULL;
                if (n[_i_1].array_of_struct)
                {
                    for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                    {
                        oe_free(n[_i_1].array_of_struct[_i_2].ptr);
                        n[_i_1].array_of_struct[_i_2].ptr = NULL;
                    }
                }
                oe_free(n[_i_1].array_of_struct);
                n[_i_1].array_of_struct = NULL;
            }
        }
    }
    /* Free nested buffers allocated by the host. */
    if (_pargs_out->n)
    {
        for (size_t _i_1 = 0; _i_1 < 1; _i_1++)
        {
            oe_host_free(_pargs_out->n[_i_1].array_of_int);
            _pargs_out->n[_i_1].array_of_int = NULL;
            if (_pargs_out->n[_i_1].array_of_struct)
            {
                for (size_t _i_2 = 0; _i_2 < 3; _i_2++)
                {
                    oe_host_free(_pargs_out->n[_i_1].array_of_struct[_i_2].ptr);
                    _pargs_out->n[_i_1].array_of_struct[_i_2].ptr = NULL;
                }
            }
            oe_host_free(_pargs_out->n[_i_1].array_of_struct);
            _pargs_out->n[_i_1].array_of_struct = NULL;
        }
    }
    return _result;
}