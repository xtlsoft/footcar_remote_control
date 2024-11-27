import { ref, type Ref, watch } from 'vue';
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
  globalServer.value = server;
  onDisconnectServer(() => {
    globalServer.value = null;
  });
  return server;
}

let globalServer: Ref<BluetoothRemoteGATTServer | null> = ref(null);

function useServer(): BluetoothRemoteGATTServer {
  if (!globalServer.value) {
    throw new Error('Server is not connected.');
  }
  return globalServer.value;
}

function onDisconnectServer(callback: () => void): void {
  if (!globalServer.value) {
    return;
  }
  globalServer.value.device.addEventListener('gattserverdisconnected', () => {
    callback();
  });
}

export { connectServer, useServer, onDisconnectServer, globalServer };
