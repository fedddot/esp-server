import requests
from service_api_pb2 import ThermostatApiRequest, ThermostatApiResponse, RequestType, StatusCode

def send_start_request(service_uri, temp_setup, time_resolution_ms):
    # build ThermostatApiRequest protobuf message (see service_api.proto for reference) according to the provided parameters
    request_msg = ThermostatApiRequest()
    request_msg.request_type = RequestType.START
    request_msg.set_temperature = temp_setup
    request_msg.time_resolution_ms = time_resolution_ms

    serialized = request_msg.SerializeToString()
    headers = {'Content-Type': 'text/plain'}
    response = requests.post(
        service_uri,
        data=serialized,
        headers=headers,
        timeout=10
    )

    response_msg = ThermostatApiResponse()
    response_msg.ParseFromString(response.content)
    return response_msg

def send_stop_request(service_uri):
    request_msg = ThermostatApiRequest()
    request_msg.request_type = RequestType.STOP
    request_msg.set_temperature = 0.0
    request_msg.time_resolution_ms = 0

    serialized = request_msg.SerializeToString()
    headers = {'Content-Type': 'text/plain'}
    response = requests.post(
        service_uri,
        data=serialized,
        headers=headers,
        timeout=10
    )

    response_msg = ThermostatApiResponse()
    response_msg.ParseFromString(response.content)
    return response_msg

# test the function with service_uri=http://192.168.1.129:5555/test_route
if __name__ == "__main__":
    service_uri = "http://192.168.1.129:5555/test_route"
    temp_setup = 22.5  # example temperature setup
    time_resolution_ms = 1000  # example time resolution
    response = send_start_request(service_uri, temp_setup, time_resolution_ms)
    print("Received start response:", response)
    response = send_stop_request(service_uri)
    print("Received stop response:", response)