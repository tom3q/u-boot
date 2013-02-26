#ifndef __LINUX_COMPAT_H__
#define __LINUX_COMPAT_H__

#define cond_resched()
#define spin_lock(x)
#define spin_unlock(x)
#define set_current_state(x)
#define schedule()
#define wake_up(x)
#define init_waitqueue_head(x)
#define spin_lock_init(x)
#define pr_debug(args...)	MTDDEBUG(MTD_DEBUG_LEVEL3, args)
#define EXPORT_SYMBOL(x)

#endif /* __LINUX_COMPAT_H__ */
