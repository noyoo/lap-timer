#include "lapTimer_http.h"

httpd_handle_t http_server;
esp_http_client_handle_t http_client;
void http_server_initialize(void) {
    httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();

    static const httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_uri_handler,
        .user_ctx = NULL,
    };

    static const httpd_uri_t cmd_uri = {
        .uri = "/command",
        .method = HTTP_POST,
        .handler = cmd_uri_handler,
        .user_ctx = NULL,
    };

    static const httpd_uri_t update_uri = {
        .uri = "/update",
        .method = HTTP_GET,
        .handler = update_uri_handler,
        .user_ctx = NULL,
    };

    static const httpd_uri_t datastream_uri = {
        .uri = "/datastream",
        .method = HTTP_POST,
        .handler = datastream_uri_handler,
        .user_ctx = NULL,
    };

    ESP_ERROR_CHECK(httpd_start(&http_server, &http_config));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &root_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &cmd_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &update_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &datastream_uri));
}

static esp_err_t root_uri_handler(httpd_req_t* request) {
    const char* response = "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>AGH Racing lap timer</title>\n    <style>\n        :root {\n            --button-unpressed-gray: #F9F9F9;\n            --button-hover-gray: #F5F5F5;\n            --indicator-red: red;\n            --indicator-green: green;\n            --indicator-blue: blue;\n            --indicator-orange: orange;\n            --indicator-black: black;\n        }\n\n        * {\n            /* box-sizing: border-box; */\n            font-family: \'Trebuchet MS\', sans-serif;\n        }\n\n        body {\n            margin: 0;\n            background: rgb(200, 200, 200);\n            background: radial-gradient(circle, rgba(200, 200, 200, .2) 0%, rgba(255, 255, 255, 1) 100%);\n            background-attachment: fixed;\n        }\n\n        button {\n            cursor: pointer;\n            border: 0;\n            display: flex;\n            flex-direction: row;\n            align-items: center;\n            gap: 1em;\n        }\n\n        header {\n            display: flex;\n            flex-direction: row;\n            align-items: center;\n            width: max-content;\n            box-shadow: .25em .25em .8em -.25em #888;\n            padding: .5em 2em .5em 1em;\n            font-size: 1.75rem;\n            color: #000;\n            border: 0.5em solid var(--button-unpressed-gray);\n            background: var(--button-unpressed-gray);\n            border-bottom-right-radius: 3em;\n        }\n\n        .content-wrapper {\n            width: 100%;\n            position: absolute;\n            display: grid;\n            grid-template-columns: 6fr 4fr;\n            gap: 1em;\n            padding: 1em;\n        }\n\n        .split-table {\n            width: 100%;\n            border-collapse: collapse;\n            font-size: 1rem;\n            text-align: center;\n            border-radius: 2em;\n            overflow: hidden;\n        }\n\n        tr {\n            height: 3em;\n        }\n\n        .split-table>thead>tr {\n            background: #DDD;\n        }\n\n        .split-table>tbody>tr:nth-last-child(even) {\n            background: #EEE;\n        }\n\n        .split-table>tbody>tr:nth-last-child(odd) {\n            background: #F5F5F5;\n        }\n\n        .split-remove-btn {\n            width: 3em !important;\n            height: 3em !important;\n            padding: 0 !important;\n            display: block !important;\n        }\n\n        .control-panel {\n            display: grid;\n            gap: 1em;\n            grid-template-columns: 1fr 1fr 1fr;\n        }\n\n        card {\n            width: 12em;\n            height: 12em;\n            display: flex;\n            flex-direction: column;\n            align-items: center;\n            justify-content: center;\n            gap: 1em;\n            background: #FFF;\n            box-shadow: .25em .25em .5em -.25em #111;\n            border-radius: 2em;\n            color: #000;\n            font-weight: 500;\n        }\n\n        .control-btn {\n            box-sizing: border-box;\n            height: 3em;\n\n            background: var(--button-unpressed-gray);\n            border: .5em solid var(--button-unpressed-gray);\n            box-shadow: .25em .25em .5em -.25em #111;\n            border-radius: 3em;\n            padding: .5em 2em;\n            color: red;\n            font-weight: 700;\n            transition-duration: 100ms;\n        }\n\n        .control-btn:hover {\n            box-shadow: .25em .25em .5em -.25em #111, inset .25em .25em 1em -.5em #222;\n            border: .5em solid var(--button-unpressed-gray);\n            background: var(--button-hover-gray);\n        }\n\n        .control-btn:active {\n            padding: 1em 2.5em;\n            box-shadow: -.25em -.25em .5em -.25em #111, inset .25em .25em 1em -.5em #222;\n            /* box-shadow: inset .25em .25em 1em -.5em #222; */\n            border: 0px solid var(--button-unpressed-gray);\n        }\n\n        .indicator {\n            --indicator-color: var(--indicator-black);\n            content: \"\";\n            width: .5em;\n            height: .5em;\n            border-radius: .5em;\n            background: var(--indicator-color);\n            box-shadow: 0 0 .5em .1em var(--indicator-color);\n        }\n    </style>\n</head>\n\n<body>\n    <header>AGH Racing lap timer</header>\n    <div class=\"content-wrapper\">\n        <div class=\"split-table-container\">\n            <table class=\"split-table\">\n                <thead>\n                    <tr class=\"table-header\">\n                        <th>Time</th>\n                        <th>Segment time</th>\n                        <th>Prev. segment diff.</th>\n                        <th>Controls</th>\n                    </tr>\n                </thead>\n                <tbody>\n                </tbody>\n            </table>\n        </div>\n        <div class=\"control-panel\">\n            <card class=\"system-control\">\n                <span>System</span>\n                <button class=\"control-btn system-control-btn start\">START<div class=\"indicator\"></div></button>\n                <button class=\"control-btn system-control-btn export\">EXPORT</button>\n            </card>\n            <card class=\"gate-control\">\n                <span class=\"gate-id\">Gate 1</span>\n                <button class=\"control-btn gate-control-btn calibration\">CALIBRATE <div class=\"indicator\"></div>\n                </button>\n                <button class=\"control-btn gate-control-btn activation\">ACTIVATE<div class=\"indicator\"></div></button>\n            </card>\n        </div>\n    </div>\n\n    <script type=\"text/javascript\">\n        let lastLength = 0;\n        let slavesRegistered = 0;\n        document.querySelectorAll(\'button\').forEach((obj) => {\n            obj.addEventListener(\'click\', buttonEventHandler);\n        });\n\n        updateCycle = setInterval(getUpdate, 500);\n\n        async function getUpdate() {\n            const r = await fetch(\'/update\');\n            status = r.headers.get(\'System-Status\');\n            response = await r.json();\n            console.log(response);\n            parseSplits(response.splits);\n            updateIndicators(response.gates, response.measurmentStarted);\n        }\n\n        function parseSplits(splits) {\n            if (splits === null) return;\n\n            start = splits[0];\n            splits.forEach((split, index, array) => {\n                if (index <= lastLength) return;\n                if (index === 0) return;\n                firstRow = document.querySelector(\'.split-table>tbody>tr\');\n                if (index === 1) {\n                    if (firstRow != null) return;\n                    el = `<tr>\n                            <td class=\"split-time\">`+ microsToTime(split - start) + `</td>\n                            <td class=\"segment-time\">`+ microsToTime(split - array[index - 1]) + `</td>\n                            <td class=\"segment-loss\"></td>\n                            <td>\n                                <button class=\"control-btn split-remove-btn\">X</button>\n                                </td>\n                                </tr>`;\n                    newRow = document.createElement(\'tr\');\n                    newRow.innerHTML = el;\n                    newRow.addEventListener(\'click\', buttonEventHandler);\n                    document.querySelector(\'.split-table>tbody\').appendChild(newRow);\n\n                    return;\n\n                }\n                if (index > 1) {\n                    el = `<tr>\n                            <td class=\"split-time\">`+ microsToTime(split - start) + `</td>\n                            <td class=\"segment-time\">`+ microsToTime(split - array[index - 1]) + `</td>\n                            <td class=\"segment-loss\">`+ microsToTime((array[index - 2] - array[index - 1]) - (array[index - 1] - split)) + `</td>\n                            <td>\n                                <button class=\"control-btn split-remove-btn\">X</button>\n                            </td>\n                        </tr>`;\n                }\n                newRow = document.createElement(\'tr\');\n                newRow.innerHTML = el;\n                newRow.addEventListener(\'click\', buttonEventHandler);\n                parent = document.querySelector(\'.split-table>tbody>tr\').parentNode;\n                parent.insertBefore(newRow, firstRow);\n                lastLength = index;\n            });\n            formatSplits();\n        }\n\n        function formatSplits() {\n            document.querySelectorAll(\'.segment-loss\').forEach((element) => {\n                if (element.innerHTML > 0) {\n                    element.style.color = \'red\';\n                } else if (element.innerHTML < 0) {\n                    element.style.color = \'green\';\n                }\n            });\n        }\n\n        function buttonEventHandler(event) {\n            if (event.target.classList.contains(\'indicator\')) {\n                alert(\'tooltip placeholder\');\n            }\n            else if (event.target.classList.contains(\'split-remove-btn\')) {\n                table = document.querySelector(\'.split-table>tbody\');\n                cnt = 0;\n                table.childNodes.forEach((child, index) => {\n                    if (child.nodeName != \'#text\') cnt++;\n                    if (child === event.target.parentNode.parentNode) {\n                        splitIndex = table.childElementCount - cnt + 1;\n                        sendSplitRemovalCmd(splitIndex);\n                    }\n                });\n            } else if (event.target.classList.contains(\'start\')) {\n                console.log(\'linku stato\');\n                sendStartCommand();\n            } else if (event.target.classList.contains(\'export\')) {\n                console.log(\'eksupotu\');\n                sendExportCommand();\n            } else if (event.target.classList.contains(\'calibration\')) {\n                id = event.target.parentElement.children[0].innerHTML.replace(\"Gate \", \"\");\n                sendGateCalibrationCmd(id);\n            } else if (event.target.classList.contains(\'activation\')) {\n                id = event.target.parentElement.children[0].innerHTML.replace(\"Gate \", \"\");\n                sendGateActivationCmd(id);\n            }\n        }\n\n        async function sendSplitRemovalCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'remove-split\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendGateCalibrationCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'calibrate-gate\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendGateActivationCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'activate-gate\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendStartCommand() {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'start-measurment\',\n                }\n            });\n        }\n\n        async function sendExportCommand() {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'export\',\n                }\n            });\n            lastLength = 0;\n            document.querySelector(\'.split-table>tbody\').innerHTML = \'\';\n        }\n\n        function updateIndicators(gates, measurmentStarted) {\n            color = measurmentStarted ? \'green\' : \'red\';\n            document.querySelector(\'.system-control>.start>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-\' + color + \')\');\n            gates.forEach((gateState, index, states) => {\n                if (index > slavesRegistered) {\n                    slavesRegistered = index;\n                    elementContent = `\n                            <span class=\"gate-id\">Gate `+ (index + 1) + `</span>\n                            <button class=\"control-btn gate-control-btn calibration\">CALIBRATE <div class=\"indicator\"></div>\n                            </button>\n                            <button class=\"control-btn gate-control-btn activation\">ACTIVATE<div class=\"indicator\"></div></button>\n                        `;\n                    element = document.createElement(\'card\');\n                    element.innerHTML = elementContent;\n                    element.classList.add(\'gate-control\');\n                    element.addEventListener(\'click\', buttonEventHandler);\n                    document.querySelector(\'.control-panel\').appendChild(element);\n                }\n                switch (gateState.state) {\n                    case 0:\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.calibration>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-blue)\');\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.activation>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-red)\');\n                        break;\n                    case 1:\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.calibration>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-green)\');\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.activation>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-orange)\');\n                        break;\n                    default:\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.calibration>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-green)\');\n                        document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')>.activation>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-green)\');\n                        break;\n                }\n\n                if (gateState.isAlive === 0) document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')\').style.display = \'none\';\n                else document.querySelector(\'.gate-control:nth-of-type(\' + (index + 2) + \')\').style.display = \'flex\';\n            });\n        }\n\n        function microsToTime(micros) {\n            hundredths = Math.floor(Math.abs(micros / 10000) % 10000);\n            seconds = Math.floor(Math.abs(micros / 1000000) % 60);\n            minutes = Math.floor(Math.abs(micros / 60000000) % 60);\n            if (minutes === 0) {\n                outputString = String(seconds).padStart(2, \'0\') + \'.\' + String(hundredths).padStart(3, \'0\');\n            } else {\n                outputString = String(minutes).padStart(2, \'0\') + \':\' + String(seconds).padStart(2, \'0\') + \'.\' + String(hundredths).padStart(2, \'0\');\n            }\n            if (micros < 0) outputString = \'-\' + outputString;\n            return outputString;\n        }\n    </script>\n</body>\n\n</html>";
    httpd_resp_send(request, response, strlen(response));

    return ESP_OK;
}

static esp_err_t cmd_uri_handler(httpd_req_t* request) {
    size_t len = httpd_req_get_hdr_value_len(request, "Command-Type") + 1;
    char buf[len];
    httpd_req_get_hdr_value_str(request, "Command-Type", buf, len);
    if (strcmp(buf, "remove-split") == 0) {
        char requestData[request->content_len];
        httpd_req_recv(request, requestData, request->content_len);
        uint8_t whichSplit = atoi(requestData);
        removeSplit(&splits, whichSplit);
    } else if (strcmp(buf, "calibrate-gate") == 0) {
        char requestData[request->content_len];
        httpd_req_recv(request, requestData, request->content_len);
        uint8_t whichGate = atoi(requestData);
        if (whichGate == 1)
            IR_sensor._state = Intercept;
        else
            slaves.slave[whichGate - 2]._instruction = 2;
    } else if (strcmp(buf, "activate-gate") == 0) {
        char requestData[request->content_len];
        httpd_req_recv(request, requestData, request->content_len);
        uint8_t whichGate = atoi(requestData);
        if (whichGate == 1)
            IR_sensor._isActive = !IR_sensor._isActive;
        else
            slaves.slave[whichGate - 2]._instruction = 1;
    } else if (strcmp(buf, "start-measurment") == 0) {
        measurmentStarted = !measurmentStarted;
    } else if (strcmp(buf, "export") == 0){
        clearAllSplits(&splits);
    }
    httpd_resp_send(request, "", 0);
    return ESP_OK;
}

static esp_err_t update_uri_handler(httpd_req_t* request) {
    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr_chunk(request, "{");

    char* leadSplits = "\"splits\":[";
    httpd_resp_sendstr_chunk(request, leadSplits);
    if (splits._position >= 0) {
        for (int i = 0; i < splits._position; i++) {
            char* buffer = NULL;
            uint8_t ret = asprintf(&buffer, "%lld,", splits._split[i]);
            if (ret > 0) {
                httpd_resp_sendstr_chunk(request, buffer);
            }
            free(buffer);
        }
        char* buffer = NULL;
        uint8_t ret = asprintf(&buffer, "%lld", splits._split[splits._position]);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
    }
    httpd_resp_sendstr_chunk(request, "],");
    char* leadGates = "\"gates\":[";
    httpd_resp_sendstr_chunk(request, leadGates);

    char* leadSingleGate = "{\"state\":"; 
            httpd_resp_sendstr_chunk(request, leadSingleGate);

    int8_t ret = 0;
    char* selfState = NULL;
    ret = asprintf(&selfState, "%d,", (uint8_t)IR_sensor._state);
    if (ret > 0) {
        httpd_resp_sendstr_chunk(request, selfState);
    }
    free(selfState);

    char* midSingleGate = "\"isAlive\":";
            httpd_resp_sendstr_chunk(request, midSingleGate);

    char* selfAlive = NULL;
    ret = asprintf(&selfAlive, "%d},",(uint8_t)1);
    if (ret > 0) {
        httpd_resp_sendstr_chunk(request, selfAlive);
    }
    free(selfAlive);

    for (int i = 0; i < slaves.pointer; i++) {
            httpd_resp_sendstr_chunk(request, leadSingleGate);
        char* buffer = NULL;
        int8_t ret = asprintf(&buffer, "%d,", (uint8_t)slaves.slave[i]._IRstate);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
        free(buffer);
            httpd_resp_sendstr_chunk(request, midSingleGate);
            buffer = NULL;
        ret = asprintf(&buffer, "%d},", (uint8_t)slaves.slave[i]._isAlive);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
        free(buffer);
    }

        httpd_resp_sendstr_chunk(request, leadSingleGate);
        char* buffer = NULL;
         ret = asprintf(&buffer, "%d,", (uint8_t)slaves.slave[slaves.pointer]._IRstate);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
        free(buffer);
            httpd_resp_sendstr_chunk(request, midSingleGate);
            buffer = NULL;
        ret = asprintf(&buffer, "%d}", (uint8_t)slaves.slave[slaves.pointer]._isAlive);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
        free(buffer);

    httpd_resp_sendstr_chunk(request, "],");
    httpd_resp_sendstr_chunk(request, "\"measurmentStarted\":");
    if (measurmentStarted)
        httpd_resp_sendstr_chunk(request, "true");
    else
        httpd_resp_sendstr_chunk(request, "false");
    httpd_resp_sendstr_chunk(request, "}");
    httpd_resp_send_chunk(request, "", 0);
    return ESP_OK;
}

static esp_err_t datastream_uri_handler(httpd_req_t* request) {
    size_t len_IP = httpd_req_get_hdr_value_len(request, "SlaveIP") + 1;
    if (len_IP > 0) {
        char SlaveIP_string[len_IP];
        httpd_req_get_hdr_value_str(request, "SlaveIP", SlaveIP_string, len_IP);
        uint32_t SlaveIP = atol(SlaveIP_string);
        size_t len_IRstate = httpd_req_get_hdr_value_len(request, "IRstate") + 1;
        char IRstate_string[len_IRstate];
        httpd_req_get_hdr_value_str(request, "IRstate", IRstate_string, len_IRstate);
        uint8_t IRstate = atoi(IRstate_string);
        int8_t slaveID = checkIfSlaveRegistered(SlaveIP);
        if (slaveID >= 0)
            updateSlave(SlaveIP, (IR_State_t)IRstate);
        else
            registerSlave(SlaveIP, (IR_State_t)IRstate);

        if (request->content_len > 0 && measurmentStarted) {
            char requestData[request->content_len];
            httpd_req_recv(request, requestData, request->content_len);
            int64_t time = atoll(requestData);
            printf("recieved time S:%s I:%lld\n, expect: %lld\n", requestData, time, getSyncedTime());
            addSplit(&splits, time);
        }
        char* resp = NULL;
        int8_t ret = asprintf(&resp, "%d", slaves.slave[slaveID]._instruction);
        if (ret > 0) {
            httpd_resp_sendstr(request, resp);
            slaves.slave[slaveID]._instruction = 0;
        }
        free(resp);
    }
    return ESP_OK;
}

void kill_http_server(void) {
    httpd_stop(http_server);
}

void http_client_initialize(void) {
    static const esp_http_client_config_t httpClientConfig = {
        .url = "http://192.168.4.1/datastream",
        .method = HTTP_METHOD_POST,
    };
    http_client = esp_http_client_init(&httpClientConfig);
    char* buf = NULL;
    asprintf(&buf, "%ld", deviceIP);
    esp_http_client_set_header(http_client, "SlaveIP", buf);
    free(buf);
}

void http_client_send_data(int64_t time) {
    char* data = NULL;
    int8_t ret = asprintf(&data, "%lld", time);
    printf("sending time I: %lld, S:%s\n", time, data);
    if (ret > 0) {
        esp_http_client_open(http_client, ret);
        esp_http_client_write(http_client, data, ret);
        esp_http_client_close(http_client);
    }
    free(data);
}

void http_client_poll(void) {
    int8_t ret = 0;
    char* buf = NULL;
    ret = asprintf(&buf, "%d", (uint8_t)IR_sensor._state);
    if (ret > 0) {
        esp_http_client_set_header(http_client, "IRstate", buf);
    }
    free(buf);
    esp_http_client_open(http_client, 0);
    int64_t contentLength = esp_http_client_fetch_headers(http_client)+1;

    char data[contentLength];
    esp_http_client_read(http_client, data, contentLength);
    data[contentLength-1] = '\0';
    uint8_t command = atoi(data);
    switch (command) {
        case 1:
            IR_sensor._isActive = !IR_sensor._isActive;
            break;
        case 2:
            IR_sensor._state = Intercept;
            break;
        default:
            break;
    }

    esp_http_client_close(http_client);
}

void kill_http_client(void) {
    esp_http_client_cleanup(http_client);
}