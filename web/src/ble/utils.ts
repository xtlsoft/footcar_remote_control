function int16ToBuffer(value: number): ArrayBuffer {
  const buffer = new Int16Array(1);
  buffer[0] = value;
  return buffer.buffer;
}

class DelayedMutex {
  private _locked: boolean = false;
  private _queue: (() => void)[] = [];
  private _delay_ms: number = 0;

  constructor(delay_ms: number) {
    this._delay_ms = delay_ms;
  }

  async acquire(): Promise<void> {
    if (this._locked) {
      await new Promise<void>((resolve) => this._queue.push(resolve));
    }
    this._locked = true;
  }

  release(): void {
    if (this._queue.length > 0) {
      const resolve = this._queue.shift();
      setTimeout(() => resolve!(), this._delay_ms);
    } else {
      this._locked = false;
    }
  }

  locked(): boolean {
    return this._locked;
  }
}

class Debounce {
  private _newest_call: Function | null = null;
  private _running: boolean = false;
  private _promise: Promise<void> | null = null;

  constructor() { }

  async call(callback: Function): Promise<void> {
    if (this._running) {
      this._newest_call = callback;
    } else {
      this._promise = new Promise<void>(async (resolve) => {
        this._running = true;
        await callback();
        (async () => {
          while (this._newest_call) {
            const newest_call = this._newest_call;
            this._newest_call = null;
            await newest_call();
          }
          this._running = false;
        })();
        resolve();
      });
    }
    await this._promise;
  }
}

export { int16ToBuffer, DelayedMutex, Debounce };
