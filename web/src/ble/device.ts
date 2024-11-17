import { DEVICE_NAME, SERVICE_UUID_RCTRL } from "./const";

async function connectServer(): Promise<BluetoothRemoteGATTServer> {
  if (!navigator.bluetooth) {
    return Promise.reject(new Error('Web Bluetooth API is not available.'));
  }
  const device = await navigator.bluetooth.requestDevice({
    filters: [
      { name: DEVICE_NAME },
      { services: [SERVICE_UUID_RCTRL] }
    ],
  });
  const server = await device.gatt?.connect();
  if (!server) {
    throw new Error('Failed to connect to the device.');
  }
  globalServer = server;
  return server;
}

let globalServer: BluetoothRemoteGATTServer | null = null;

function useServer(): BluetoothRemoteGATTServer {
  if (!globalServer) {
    throw new Error('Server is not connected.');
  }
  return globalServer;
}

export { connectServer , useServer };
