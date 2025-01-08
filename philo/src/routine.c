#include <philo.h>

void	thinking(t_philo *philo)
{
	if (lock_unclok(philo, "thinking"))
		return ;
	usleep(philo->time_to_eat * 1000);
}

void	eating(t_philo *philo)
{
	if (lock_unclok(philo, "eating"))
		return ;
	usleep(philo->time_to_eat * 1000);
	philo->last_meal_time = current_time_ms();
	philo->meals_eaten++;
}

void	sleeping(t_philo *philo)
{
	if (lock_unclok(philo, "sleeping"))
		return ;
	usleep(philo->time_to_sleep * 1000);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->table->print_lock);
		if (philo->table->should_stop)
		{
			pthread_mutex_unlock(&philo->table->print_lock);
			return (NULL);
		}
		if ((current_time_ms() - philo->last_meal_time) >= philo->time_to_die)
		{
			philo->table->should_stop = 1;
			printf("%ld %d died\n", current_time_ms(), philo->id);
			pthread_mutex_unlock(&philo->table->print_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->table->print_lock);
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
}
