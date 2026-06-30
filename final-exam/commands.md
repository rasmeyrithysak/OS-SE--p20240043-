# commands.md — exact commands I ran, per part

> Paste the **real** commands you ran, in order, in the fenced blocks below. Graded for
> command competency and is your defence if any output is questioned. One block per part.
> Delete the hint comments and replace with your actual commands.

## Part A — Threads, Mapping & Signals

```bash
# compile the threaded program (mind the threading flag), run it
# capture the 1:1 user→kernel (LWP) mapping into thread_map.txt while it runs
# compile/run signal_demo and demonstrate catching the interactive interrupt
gcc -pthread thread_demo.c -o thread_demo
./thread_demo

ps -L -p $(pgrep -x thread_demo) > thread_map.txt
cat thread_map.txt

gcc signal_demo.c -o signal_demo
./signal_demo
(sent SIGINT via Ctrl+C)
```

## Part B — Permissions, Special Bits & ACLs

```bash
# build the tree (shared dir + private file); set octal + symbolic modes
# demonstrate setgid + sticky on a dir you own; build/set the setuid binary
# add and read back an ACL entry; save reports
mkdir -p shared_dir
touch private_file.txt
chmod 600 private_file.txt
chmod 711 shared_dir
ls -l private_file.txt
ls -ld shared_dir
stat private_file.txt
stat shared_dir

mkdir -p special_demo
chmod g+s special_demo
chmod +t special_demo
ls -ld special_demo
touch special_demo/testfile.txt
ls -l special_demo/

gcc setuid_demo.c -o setuid_demo
chmod u+s setuid_demo
ls -l setuid_demo
./setuid_demo
```

## Part C — Bash Scripting, PATH & Safe Scanning

```bash
# make greeter runnable by name via PATH; record PATH + resolved location
# run collector over your dirs; show it skips unreadable/missing files safely
nano scripts/greeter
chmod +x scripts/greeter
cp scripts/greeter ~/bin/greeter
greeter

echo "$PATH" 
which greeter
type greeter

mkdir -p data1 data2 data3
echo "Sales report for OrbitWorks Q1" > data1/file1.txt
echo "Widget inventory notes" > data2/file2.txt
echo "secret stuff" > data3/file3.txt
chmod 000 data3/file3.txt

nano scripts/collector
chmod +x scripts/collector
./scripts/collector
cat collector_report.txt
```

## Part D — Race Condition & flock

```bash
# init stock; run swarm several times unpatched and record final stock each time
# add the exclusive advisory lock around the read-modify-write; re-run swarm
echo 200 > stock.txt
nano scripts/buy_widget
chmod +x scripts/buy_widget
./scripts/buy_widget Alice 5

nano scripts/swarm
chmod +x scripts/swarm

# D2 - unpatched race, 5 runs
for run in 1 2 3 4 5; do
  echo 200 > stock.txt; rm -f sales.log; ./scripts/swarm > /dev/null
  echo "Run $run: final stock = $(cat stock.txt)"
done

# D3 - patched buy_widget with flock -x on fd 200 bound to stock.lock
nano scripts/buy_widget   # added flock -x 200 ... 200>"$LOCK_FILE"
for run in 1 2 3 4 5; do
  echo 200 > stock.txt; rm -f sales.log; ./scripts/swarm > /dev/null
  echo "Patched run $run: final stock = $(cat stock.txt)"
done
```

## Part E — Backups & cron

```bash
# E1: run backup_project enough times that pruning happens (keep newest RETAIN_N)
# E2: per-user crontab, two entries (absolute paths):
#     recurring (CRON_INTERVAL) -> partE_automation/logs/cron_recurring.log
#     one-shot at TIMED        -> partE_automation/logs/cron_oneshot.log
# E3: backup_exam -> tar the final-exam folder to ~/exam-backups/final-exam-<ts>.tar.gz
#     crontab: run backup_exam on a short interval AND once at exactly 16:00 today
#     then: ls ~/exam-backups
# capture crontab -l + both logs + the ~/exam-backups listing into cron_report.txt
<your commands>
```
