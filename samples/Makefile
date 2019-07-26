
#SUBDIRS = bulk_memory_pool \
#			heap_functions \
#			heap_sort \
#			mt_queue \
#			priority_queue \
#			priority_queue_ref \
#			spin_lock \
#			stopwatch \
#			task_queue \
#			worker_thread

# 在shell中执行: find . -name "Makefile" | awk '{print substr($0, 3, length($0)-2-length("Makefile"))}' | grep -v '^$'
# 找到所有包含Makefile的路径
# 由于$在Makefile里有特殊含义, 所以必须用$$转义.
SUBDIRS = $(shell find . -name 'Makefile' | awk '{print substr($$0, 3, length($$0)-2-length("Makefile"))}' | grep -v '^$$')

all:
	@echo $(SUBDIRS)
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

clean:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

common:
	@for dir in $(SUBDIRS); do \
		echo "Making $(WHAT_TO_MAKE) in $$dir..."; \
		(cd $$dir && $(MAKE) $(MFLAGS) $(WHAT_TO_MAKE)) || exit 1; \
	done
