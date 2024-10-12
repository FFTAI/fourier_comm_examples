# fourier_comm_examples

## cpp bindings for the fourier_comm library

```bash
cd cpp && mkdir build && cd build
cmake .. && make
./example
```

You should see something similar to the following output:

```txt
Motor 13 posittion: 0.00898801
Motor 13 Control mode: position
Motor 13 state: 193.624µs
Motor 14 posittion: -0.0523503
Motor 14 Control mode: position
Motor 14 state: 210.514µs
Motor 15 posittion: 0.74128
Motor 15 Control mode: position
Motor 15 state: 235.477µs
```

## python bindings for the fourier_comm library

```bash
pip install fourier_comm_rs
python python/example.py
```