
SUBDIRS = bulk_memory_pool \
			heap_functions \
			heap_sort \
			mt_queue \
			priority_queue \
			priority_queue_ref \
			spin_lock \
			stopwatch \
			task_queue \
			worker_thread


all:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

clean:
	@$(MAKE) $(MFLAGS) WHAT_TO_MAKE=$@ common

common:
	@for dir in $(SUBDIRS); do \
		echo "Making $(WHAT_TO_MAKE) in $$dir..."; \
		(cd $$dir && $(MAKE) $(MFLAGS) $(WHAT_TO_MAKE)) || exit 1; \
	done
