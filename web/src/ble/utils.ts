function int16ToBuffer(value: number): ArrayBuffer {
  const buffer = new Int16Array(1);
  buffer[0] = value;
  return buffer.buffer;
}

export { int16ToBuffer };
