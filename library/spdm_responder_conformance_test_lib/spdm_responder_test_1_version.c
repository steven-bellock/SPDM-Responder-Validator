/**
 *  Copyright Notice:
 *  Copyright 2021 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/spdm-emu/blob/main/LICENSE.md
 **/

#include "spdm_responder_test.h"

bool spdm_test_case_version_setup_version (void *test_context)
{
    return true;
}

void spdm_test_case_version_success (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_version_request_t spdm_request;
    spdm_version_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_version_number_t *version_number_entry;
    size_t spdm_response_size;
    common_test_result_t test_result;
    size_t index;
    spdm_version_number_t version;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    spdm_request.header.spdm_version = SPDM_MESSAGE_VERSION_10;
    spdm_request.header.request_response_code = SPDM_GET_VERSION;
    spdm_request.header.param1 = 0;
    spdm_request.header.param2 = 0;

    spdm_response = (void *)message;
    spdm_response_size = sizeof(message);
    libspdm_zero_mem(message, sizeof(message));
    status = libspdm_send_receive_data(spdm_context, NULL, false,
                                       &spdm_request, sizeof(spdm_request),
                                       spdm_response, &spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10,
            0,
            COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
        return;
    }

    if (spdm_response_size > sizeof(spdm_version_response_t)) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10, 1,
        test_result, "response size - %d", spdm_response_size);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return;
    }

    if (spdm_response->header.request_response_code == SPDM_VERSION) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10, 2,
        test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return;
    }

    if (spdm_response->header.spdm_version == SPDM_MESSAGE_VERSION_10) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10, 3,
        test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return;
    }

    if ((spdm_response->version_number_entry_count > 0) &&
        (spdm_response->version_number_entry_count <=
         (spdm_response_size - sizeof(spdm_version_response_t)) / sizeof(spdm_version_number_t))) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10, 4,
        test_result, "response version_number_entry_count - 0x%02x",
        spdm_response->version_number_entry_count);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return;
    }

    version_number_entry = (void *)(spdm_response + 1);
    for (index = 0; index < spdm_response->version_number_entry_count; index++) {
        version = version_number_entry[index] >> SPDM_VERSION_NUMBER_SHIFT_BIT;
        switch (version) {
            case SPDM_MESSAGE_VERSION_10:
            case SPDM_MESSAGE_VERSION_11:
            case SPDM_MESSAGE_VERSION_12:
            case SPDM_MESSAGE_VERSION_13:
                test_result = COMMON_TEST_RESULT_PASS;
                break;
            default:
                test_result = COMMON_TEST_RESULT_FAIL;
                break;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_VERSION, SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10, 5,
            test_result, "response version_number_entry - 0x%04x", version_number_entry[index]);
    }
}

common_test_case_t m_spdm_test_group_version[] = {
    {SPDM_RESPONDER_TEST_CASE_VERSION_SUCCESS_10,
     "spdm_test_case_version_success",
     spdm_test_case_version_success,
     spdm_test_case_version_setup_version,
     spdm_test_case_common_teardown},
    {COMMON_TEST_ID_END, NULL, NULL},
};
