可直接执行AutoCompile.bat自动编译运行（要求cmake、MinGW），或手动使用常规cmake方式编译

生成的目标程序在./build路径下


为满足“通用程序”要求，同时更清晰地标识各部分结构，这里使用工程形式组织程序，主要是这个要求也确实适合单接口多实现

.
├── AutoCompile.bat		<--------------自动运行脚本
├── CMakeLists.txt
├── Main.cc				<--------------程序入口
├── config                            <--------------配置了数据精度与对应的math函数，默认long double
│   └── Config.hh
├── include
│   ├── NewtonSolver.hh
│   ├── SecantSolver.hh
│   └── Solver.hh	                <--------------求解器接口声明，请优先看这里
└── src
    ├── NewtonSolver.cc
    ├── SecantSolver.cc
    └── Solver.cc