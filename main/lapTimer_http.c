#include "lapTimer_http.h"

    httpd_handle_t http_server;
void http_server_initialize(void) {
    httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();

    static const httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_uri_handler,
        .user_ctx = NULL,  // @suppress("Symbol is not resolved")
    };

    static const httpd_uri_t cmd_uri = {
        .uri = "/command",
        .method = HTTP_POST,
        .handler = cmd_uri_handler,
        .user_ctx = NULL,  // @suppress("Symbol is not resolved")
    };

    static const httpd_uri_t update_uri = {
        .uri = "/update",
        .method = HTTP_GET,
        .handler = update_uri_handler,
        .user_ctx = NULL,
    };

    ESP_ERROR_CHECK(httpd_start(&http_server, &http_config));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &root_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &cmd_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &update_uri));
}

static esp_err_t root_uri_handler(httpd_req_t* request) {
    const char* response = "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>AGH Racing lap timer</title>\n    <style>\n        :root {\n            --button-unpressed-gray: #F9F9F9;\n            --button-hover-gray: #F5F5F5;\n            --indicator-red: red;\n            --indicator-green: green;\n            --indicator-blue: blue;\n            --indicator-orange: orange;\n            --indicator-black: black;\n        }\n\n        * {\n            /* box-sizing: border-box; */\n            font-family: \'Trebuchet MS\', sans-serif;\n        }\n\n        body {\n            margin: 0;\n            background: rgb(200, 200, 200);\n            background: radial-gradient(circle, rgba(200, 200, 200, .2) 0%, rgba(255, 255, 255, 1) 100%);\n            background-attachment: fixed;\n        }\n\n        button {\n            cursor: pointer;\n            border: 0;\n            display: flex;\n            flex-direction: row;\n            align-items: center;\n            gap: 1em;\n        }\n\n        header {\n            display: flex;\n            flex-direction: row;\n            align-items: center;\n            width: max-content;\n            box-shadow: .25em .25em .8em -.25em #888;\n            padding: .5em 2em .5em 1em;\n            font-size: 1.75rem;\n            color: #000;\n            border: 0.5em solid var(--button-unpressed-gray);\n            background: var(--button-unpressed-gray);\n            border-bottom-right-radius: 3em;\n        }\n\n        .content-wrapper {\n            width: 100%;\n            position: absolute;\n            display: grid;\n            grid-template-columns: 6fr 4fr;\n            gap: 1em;\n            padding: 1em;\n        }\n\n        .split-table {\n            width: 100%;\n            border-collapse: collapse;\n            font-size: 1rem;\n            text-align: center;\n            border-radius: 2em;\n            overflow: hidden;\n        }\n\n        tr {\n            height: 3em;\n        }\n\n        .split-table>thead>tr {\n            background: #DDD;\n        }\n\n        .split-table>tbody>tr:nth-last-child(even) {\n            background: #EEE;\n        }\n\n        .split-table>tbody>tr:nth-last-child(odd) {\n            background: #F5F5F5;\n        }\n\n        .split-remove-btn {\n            width: 3em !important;\n            height: 3em !important;\n            padding: 0 !important;\n            display: block !important;\n        }\n\n        .control-panel {\n            display: grid;\n            gap: 1em;\n            grid-template-columns: 1fr 1fr 1fr;\n        }\n\n        card {\n            width: 12em;\n            height: 12em;\n            display: flex;\n            flex-direction: column;\n            align-items: center;\n            justify-content: center;\n            gap: 1em;\n            background: #FFF;\n            box-shadow: .25em .25em .5em -.25em #111;\n            border-radius: 2em;\n            color: #000;\n            font-weight: 500;\n        }\n\n        .control-btn {\n            box-sizing: border-box;\n            height: 3em;\n\n            background: var(--button-unpressed-gray);\n            border: .5em solid var(--button-unpressed-gray);\n            box-shadow: .25em .25em .5em -.25em #111;\n            border-radius: 3em;\n            padding: .5em 2em;\n            color: red;\n            font-weight: 700;\n            transition-duration: 100ms;\n        }\n\n        .control-btn:hover {\n            box-shadow: .25em .25em .5em -.25em #111, inset .25em .25em 1em -.5em #222;\n            border: .5em solid var(--button-unpressed-gray);\n            background: var(--button-hover-gray);\n        }\n\n        .control-btn:active {\n            padding: 1em 2.5em;\n            box-shadow: -.25em -.25em .5em -.25em #111, inset .25em .25em 1em -.5em #222;\n            /* box-shadow: inset .25em .25em 1em -.5em #222; */\n            border: 0px solid var(--button-unpressed-gray);\n        }\n\n        .indicator {\n            --indicator-color: var(--indicator-black);\n            content: \"\";\n            width: .5em;\n            height: .5em;\n            border-radius: .5em;\n            background: var(--indicator-color);\n            box-shadow: 0 0 .5em .1em var(--indicator-color);\n        }\n    </style>\n</head>\n\n<body>\n    <header>AGH Racing lap timer</header>\n    <div class=\"content-wrapper\">\n        <div class=\"split-table-container\">\n            <table class=\"split-table\">\n                <thead>\n                    <tr class=\"table-header\">\n                        <th>Time</th>\n                        <th>Segment time</th>\n                        <th>Prev. segment loss</th>\n                        <th>Controls</th>\n                    </tr>\n                </thead>\n                <tbody>\n                    <!-- <tr>\n                        <td>1:15.225</td>\n                        <td>1:15.225</td>\n                        <td>-0.334</td>\n                        <td>\n                            <button class=\"control-btn split-remove-btn\">X</button>\n                        </td>\n                    </tr>\n                    <tr>\n                        <td>1:15.225</td>\n                        <td>1:15.225</td>\n                        <td>-0.334</td>\n                        <td>\n                            <button class=\"control-btn split-remove-btn\">X</button>\n                        </td>\n                    </tr>\n                    <tr>\n                        <td>1:15.225</td>\n                        <td>1:15.225</td>\n                        <td>-0.334</td>\n                        <td>\n                            <button class=\"control-btn split-remove-btn\">X</button>\n                        </td>\n                    </tr> -->\n                </tbody>\n            </table>\n        </div>\n        <div class=\"control-panel\">\n            <card class=\"system-control\">\n                <span>System</span>\n                <button class=\"control-btn system-control-btn start\">START<div class=\"indicator\"></div></button>\n                <button class=\"control-btn system-control-btn export\">EXPORT</button>\n            </card>\n            <card class=\"gate-control\">\n                <span class=\"gate-id\">Gate 1</span>\n                <button class=\"control-btn gate-control-btn calibration\">CALIBRATE <div class=\"indicator\"></div>\n                </button>\n                <button class=\"control-btn gate-control-btn activation\">ACTIVATE<div class=\"indicator\"></div></button>\n            </card>\n        </div>\n    </div>\n\n    <script type=\"text/javascript\">\n        let lastLength = 0;\n        document.querySelectorAll(\'button\').forEach((obj) => {\n            obj.addEventListener(\'click\', buttonEventHandler);\n        });\n\n        updateCycle = setInterval(getUpdate, 1000);\n\n        async function getUpdate() {\n            const r = await fetch(\'/update\');\n            status = r.headers.get(\'System-Status\');\n            response = await r.json();\n            parseSplits(response.splits);\n            updateIndicators(status);\n        }\n\n        function parseSplits(splits) {\n            if(splits === null) return;\n\n            start = splits[0];\n            splits.forEach((split, index, array) => {\n                if (index <= lastLength) return;\n                if (index === 0) return;\n                if (index === 1) {\n                    el = `<tr>\n                            <td class=\"split-time\">`+ split + `</td>\n                            <td class=\"segment-time\">`+ (array[index - 1] - split) + `</td>\n                            <td class=\"segment-loss\"></td>\n                            <td>\n                                <button class=\"control-btn split-remove-btn\">X</button>\n                            </td>\n                        </tr>`;\n\n                }\n                if (index > 1) {\n                    el = `<tr>\n                            <td class=\"split-time\">`+ split + `</td>\n                            <td class=\"segment-time\">`+ (array[index - 1] - split) + `</td>\n                            <td class=\"segment-loss\">`+ ((array[index - 2] - array[index - 1]) - (array[index - 1] - split)) + `</td>\n                            <td>\n                                <button class=\"control-btn split-remove-btn\">X</button>\n                            </td>\n                        </tr>`;\n                }\n                newRow = document.createElement(\'tr\');\n                newRow.innerHTML = el;\n                newRow.addEventListener(\'click\', buttonEventHandler);\n                firstRow = document.querySelector(\'.split-table>tbody>tr\');\n                if (firstRow === null) {\n                    document.querySelector(\'.split-table>tbody\').appendChild(newRow);\n                    return;\n                }\n                parent = document.querySelector(\'.split-table>tbody>tr\').parentNode;\n                parent.insertBefore(newRow, firstRow);\n                lastLength = index;\n            });\n            formatSplits();\n        }\n\n        function formatSplits() {\n            document.querySelectorAll(\'.segment-loss\').forEach((element) => {\n                if (element.innerHTML > 0) {\n                    element.style.color = \'red\';\n                } else if (element.innerHTML < 0) {\n                    element.style.color = \'green\';\n                }\n            });\n        }\n\n        function buttonEventHandler(event) {\n            if (event.target.classList.contains(\'indicator\')) {\n                alert(\'tooltip placeholder\');\n            }\n            else if (event.target.classList.contains(\'split-remove-btn\')) {\n                table = document.querySelector(\'.split-table>tbody\');\n                cnt = 0;\n                table.childNodes.forEach((child, index) => {\n                    if (child.nodeName != \'#text\') cnt++;\n                    if (child === event.target.parentNode.parentNode) {\n                        splitIndex = table.childElementCount - cnt + 1;\n                        sendSplitRemovalCmd(splitIndex);\n                    }\n                });\n            } else if (event.target.classList.contains(\'start\')) {\n                console.log(\'linku stato\');\n                sendStartCommand();\n            } else if (event.target.classList.contains(\'export\')) {\n                console.log(\'eksupotu\');\n            } else if (event.target.classList.contains(\'calibration\')) {\n                console.log(\'kalibreto\');\n                sendGateCalibrationCmd(1);\n            } else if (event.target.classList.contains(\'activation\')) {\n                console.log(\'aktiveto\');\n                sendGateActivationCmd(1);\n            }\n        }\n\n        async function sendSplitRemovalCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'remove-split\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendGateCalibrationCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'calibrate-gate\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendGateActivationCmd(index) {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'activate-gate\',\n                },\n                body: index,\n            });\n        }\n\n        async function sendStartCommand() {\n            const r = await fetch(\'/command\', {\n                method: \'POST\',\n                headers: {\n                    \'Content-Type\': \'text/html\',\n                    \'Command-Type\': \'start-session\',\n                },\n                body: 1,\n            });\n        }\n\n        function updateIndicators(status) {\n            // system status\n            // 0bxxxxxx00 - locked\n            // 0bxxxxxx01 - ready\n            // 0bxxxxxx1x - running\n            // gate 1 calibration\n            // 0bxxxx00xx - not calibrated\n            // 0bxxxx01xx - calibrated\n            // 0bxxxx1xxx - calibration in progress\n            // gate 1 activation\n            // 0bxxx0xxxx - not activated\n            // 0bxxx1xxxx - activated\n            color = ((status & 0b00000010) === 0b00000010) ? \'green\' : ((status & 0b00000001) === 0b00000001) ? \'orange\' : \'red\';\n            document.querySelector(\'.system-control>.start>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-\' + color + \')\');\n            color = ((status & 0b00001000) === 0b00001000) ? \'blue\' : ((status & 0b00000100) === 0b00000100) ? \'green\' : \'red\';; // STATUS_GATE1_CALIBRATION_INPROGRESS_MASK\n            document.querySelector(\'.gate-control:nth-of-type(2)>.calibration>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-\' + color + \')\');\n            color = ((status & 0b00010000) === 0b00010000) ? \'green\' : \'red\';\n            document.querySelector(\'.gate-control>.activation>.indicator\').style.setProperty(\'--indicator-color\', \'var(--indicator-\' + color + \')\');\n        }\n    </script>\n</body>\n\n</html>";
    httpd_resp_send(request, response, strlen(response));

    return ESP_OK;
}

static esp_err_t cmd_uri_handler(httpd_req_t* request) {
    size_t len = httpd_req_get_hdr_value_len(request, "Command-Type");
    char* buf = malloc(len + 1);
    httpd_req_get_hdr_value_str(request, "Command-Type", buf, len + 1);
    if (strcmp(buf, "remove-split") == 0) {
        char requestData[request->content_len + 1];
        httpd_req_recv(request, requestData, request->content_len);
        uint8_t whichSplit = atoi(requestData);
        printf("%d\n", whichSplit);
        removeSplit(&splits, whichSplit);
        printf("%lld\n", splits._split[splits._position]);
    } else if (strcmp(buf, "calibrate-gate") == 0) {
        printf("karibureto\n");
    } else if (strcmp(buf, "activate-gate") == 0) {
        printf("aktiveto\n");
        setStatusBit(Gate1_active);
    } else if (strcmp(buf, "start-session") == 0) {
        printf("linku stato\n");
    }
    httpd_resp_send(request, "", 0);
    free(buf);
    return ESP_OK;
}

static esp_err_t update_uri_handler(httpd_req_t* request) {
    char* hdr = malloc(3*sizeof(char));
    itoa(_statusBits, hdr, 10);
    httpd_resp_set_hdr(request, "System-Status", hdr);
    httpd_resp_set_type(request, "application/json");
    char* lead = "{\"splits\":[";
    httpd_resp_sendstr_chunk(request, lead);
    for (int i = 0; i < splits._position; i++) {
        char* buffer = NULL;
        uint8_t ret = asprintf(&buffer, "%lld,", splits._split[i]);
        if (ret > 0) {
            httpd_resp_sendstr_chunk(request, buffer);
        }
        free(buffer);
    }
    char* buffer = NULL;
    uint8_t ret = asprintf(&buffer, "%lld]}", splits._split[splits._position]);
    if (ret > 0) {
        httpd_resp_sendstr_chunk(request, buffer);
    }
    httpd_resp_send_chunk(request, "", 0);
    free(buffer);
    return ESP_OK;
}

void kill_http(void){
    httpd_stop(http_server);
}