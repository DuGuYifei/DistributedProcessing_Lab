import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created By IDEA
 * Author: s188026 Yifei Liu
 * Date: 2023/5/18 8:46
 * Description: Main
 */

public class LabApplication {

    private final int MAX_EACH_CONSUME = 100;
    private final int NUM_CONSUMER = 1;
    private final int NUM_PRODUCER = 1;

    public class Production {
        String name;

        public Production() {
        }
    }

    public class ProductionA extends Production {
        public ProductionA() {
            this.name = "A";
        }
    }

    public class ProductionB extends Production {
        public ProductionB() {
            this.name = "B";
        }
    }

    public class ProductionC extends Production {
        public ProductionC() {
            this.name = "C";
        }
    }

    public class Monitor {
        private final List<Production> productions = new ArrayList<>();
        private final int MAX_SIZE = 10;
        private final Random rand = new Random();
        private final ReentrantLock lock = new ReentrantLock(true);
        private final Condition notFull = lock.newCondition();
        private final Condition notEmpty = lock.newCondition();
        public final AtomicInteger countProductionA = new AtomicInteger(0);
        public final AtomicInteger countProductionB = new AtomicInteger(0);
        public final AtomicInteger countProductionC = new AtomicInteger(0);

        public Monitor() {
        }

        public void produce(Production production) throws InterruptedException {
            lock.lock();
            if (productions.size() >= MAX_SIZE) {
                System.out.println("The productions are full!");
                notFull.await();
            }
            productions.add(production);
            if (production instanceof ProductionA) {
                countProductionA.incrementAndGet();
            } else if (production instanceof ProductionB) {
                countProductionB.incrementAndGet();
            } else if (production instanceof ProductionC) {
                countProductionC.incrementAndGet();
            }
            notEmpty.signal();
            lock.unlock();
        }

        public Production consume(int type) throws InterruptedException {
            lock.lock();
            if(productions.size() == 0) {
                System.out.println("The productions are empty!");
                notEmpty.await();
            }
            //int type = -1;
            //int typeCnt = 0;
            // Reservoir Sampling
//            if(countProductionA.longValue() > 0){
//                typeCnt++;
//                if(rand.nextInt(typeCnt) == 0){
//                    type = 0;
//                }
//            }
//            if(countProductionB.longValue() > 0){
//                typeCnt++;
//                if(rand.nextInt(typeCnt) == 0){
//                    type = 1;
//                }
//            }
//            if(countProductionC.longValue() > 0){
//                typeCnt++;
//                if(rand.nextInt(typeCnt) == 0){
//                    type = 2;
//                }
//            }

            Production consumedProduction = null;
            for (int i = 0; i < productions.size(); i++) {
                if (productions.get(i) instanceof ProductionA && type == 0) {
                    consumedProduction = productions.remove(i);
                    countProductionA.decrementAndGet();
                    break;
                } else if (productions.get(i) instanceof ProductionB && type == 1) {
                    consumedProduction = productions.remove(i);
                    countProductionB.decrementAndGet();
                    break;
                } else if (productions.get(i) instanceof ProductionC && type == 2) {
                    consumedProduction = productions.remove(i);
                    countProductionC.decrementAndGet();
                    break;
                }
            }
            if(consumedProduction == null){
                lock.unlock();
                return null;
            }
            notFull.signal();
            lock.unlock();
            return consumedProduction;
        }
    }

    public class Producer extends Thread {
        private final Monitor monitor;
        private final int MAX_PRODUCE = MAX_EACH_CONSUME * NUM_CONSUMER / NUM_PRODUCER;

        public Producer(Monitor monitor) {
            this.monitor = monitor;
        }

        @Override
        public void run() {
            int count = 0;
            while (count < MAX_PRODUCE) {
                count++;

                try {
                    int type = new Random().nextInt(3);
                    Production production = null;
                    if (type == 0) {
                        production = new ProductionA();
                    } else if (type == 1) {
                        production = new ProductionB();
                    } else if (type == 2) {
                        production = new ProductionC();
                    }
                    System.out.println("Produced " + production.name + " " + count);
                    monitor.produce(production);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            System.out.println("Produced " + count + " productions");
        }
    }

    public class Consumer extends Thread {
        private final Monitor monitor;
        private final int MAX_CONSUME = MAX_EACH_CONSUME;
        private final Random rand = new Random();

        public Consumer(Monitor monitor) {
            this.monitor = monitor;
        }

        @Override
        public void run() {
            int count = 0;
            while (count < MAX_CONSUME) {
                count++;
                int type = rand.nextInt(3);
                try {
                    Production consumedProduction = monitor.consume(type);
                    if (consumedProduction == null) {
                        count--;
                        System.out.println("There is no this type production!");
                        continue;
                    }
                    System.out.println("Consumed " + consumedProduction.name + " " + count);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            System.out.println("Consumed " + count + " productions");
        }
    }

    public static void main(String[] args) {
        LabApplication labApplication = new LabApplication();
        Monitor monitor = labApplication.new Monitor();
        Producer[] producers = new Producer[labApplication.NUM_PRODUCER];
        Consumer[] consumers = new Consumer[labApplication.NUM_CONSUMER];
        for (int i = 0; i < labApplication.NUM_PRODUCER; i++) {
            producers[i] = labApplication.new Producer(monitor);
        }
        for (int i = 0; i < labApplication.NUM_CONSUMER; i++) {
            consumers[i] = labApplication.new Consumer(monitor);
        }
        for (int i = 0; i < labApplication.NUM_PRODUCER; i++) {
            producers[i].start();
        }
        for (int i = 0; i < labApplication.NUM_CONSUMER; i++) {
            consumers[i].start();
        }
        // 等待线程结束
        for (int i = 0; i < labApplication.NUM_PRODUCER; i++) {
            try {
                producers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
