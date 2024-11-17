import { CHAR_UUID_MOTOR_A, CHAR_UUID_MOTOR_B, CHAR_UUID_SERVO, SERVICE_UUID_RCTRL } from './const';
import { useServer } from './device';
import { int16ToBuffer } from './utils';

async function useService(): Promise<BluetoothRemoteGATTService> {
  const server = useServer();
  return await server.getPrimaryService(SERVICE_UUID_RCTRL);
}

async function useCharacteristic(uuid: number): Promise<BluetoothRemoteGATTCharacteristic> {
  const server = await useServer();
  const service = await server.getPrimaryService(SERVICE_UUID_RCTRL);
  const characteristic = await service.getCharacteristic(uuid);
  return characteristic;
}

async function writeServo(angle: number): Promise<void> {
  const characteristic = await useCharacteristic(CHAR_UUID_SERVO);
  await characteristic.writeValue(int16ToBuffer(angle));
}

async function writeMotorA(speed: number): Promise<void> {
  const characteristic = await useCharacteristic(CHAR_UUID_MOTOR_A);
  await characteristic.writeValue(int16ToBuffer(speed));
}

async function writeMotorB(speed: number): Promise<void> {
  const characteristic = await useCharacteristic(CHAR_UUID_MOTOR_B);
  await characteristic.writeValue(int16ToBuffer(speed));
}

export { useService, useCharacteristic, writeServo, writeMotorA, writeMotorB };
