#ifndef UTILS_H
#define UTILS_H





Instrucao* gerarInstrucoes(int N_INST, int N_MEM, int N_PROB, int N_FOR, int N_OPCODE, int N_WORD);
void setupBenchmark(BenchMetrics *metrics, ConfigItem *configs);
void CacheBenchmark(BenchMetrics *metrics, ConfigItem *configs);
void testeRapido(ConfigItem *configs); 

#endif 