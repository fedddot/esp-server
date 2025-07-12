import requests
from service_api_pb2 import ThermostatApiRequest, ThermostatApiResponse, RequestType, StatusCode
from google.protobuf.json_format import MessageToDict

def run_request(service_uri: str, request: ThermostatApiRequest) -> ThermostatApiResponse:
    serialized = request.SerializeToString()
    headers = {'Content-Type': 'text/plain'}
    response = requests.post(
        service_uri,
        data=serialized,
        headers=headers,
        timeout=10
    )
    if response.status_code != 200:
        raise Exception(f"Failed to send request, status code: {response.status_code}")
    response_msg = ThermostatApiResponse()
    response_msg.ParseFromString(response.content)
    return response_msg

def print_response(response: ThermostatApiResponse):
    status_mapping = {
        StatusCode.SUCCESS: "Success",
        StatusCode.FAILURE: "Failure"
    }
    print(f"Status: {status_mapping.get(response.status, 'Unknown')}")
    print(f"Message: {response.message}")

def print_request(request: ThermostatApiRequest):
    request_type_mapping = {
        RequestType.START: "Start",
        RequestType.STOP: "Stop"
    }
    print(f"Request Type: {request_type_mapping.get(request.request_type, 'Unknown')}")
    if request.set_temperature:
        print(f"Set Temperature: {request.set_temperature}")
    if request.time_resolution_ms:
        print(f"Time Resolution (ms): {request.time_resolution_ms}")

if __name__ == "__main__":
    service_uri = "http://192.168.1.129:5555/test_route"
    temp_setup = 22.5  # example temperature setup
    time_resolution_ms = 1000  # example time resolution
    
    start_request = ThermostatApiRequest(
        request_type=RequestType.START,
        set_temperature=temp_setup,
        time_resolution_ms=time_resolution_ms
    )
    print("Sending test request:")
    print_request(start_request)
    response = run_request(service_uri, start_request)
    print("Received response:")
    print_response(response)
    
    stop_request = ThermostatApiRequest(
        request_type=RequestType.STOP
    )
    print("Sending test request:")
    print_request(stop_request)
    response = run_request(service_uri, stop_request)
    print("Received response:")
    print_response(response)