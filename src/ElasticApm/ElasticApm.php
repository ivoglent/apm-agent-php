<?php

declare(strict_types=1);

namespace Elastic\Apm;

use Closure;
use Elastic\Apm\Impl\GlobalTracerHolder;
use Elastic\Apm\Impl\Util\StaticClassTrait;
use Throwable;

/**
 * Class ElasticApm is a facade (as in Facade design pattern) to the rest of Elastic APM public API.
 */
final class ElasticApm
{
    use StaticClassTrait;

    /** @var string */
    public const VERSION = '1.0.0-beta1';

    /**
     * Begins a new transaction and sets it as the current transaction.
     *
     * @param string                      $name      New transaction's name
     * @param string                      $type      New transaction's type
     * @param float|null                  $timestamp Start time of the new transaction
     * @param DistributedTracingData|null $distributedTracingData
     *
     * @return TransactionInterface New transaction
     *
     * @see TransactionInterface::setName() For the description.
     * @see TransactionInterface::setType() For the description.
     * @see TransactionInterface::getTimestamp() For the description.
     *
     */
    public static function beginCurrentTransaction(
        string $name,
        string $type,
        ?float $timestamp = null,
        ?DistributedTracingData $distributedTracingData = null
    ): TransactionInterface {
        return GlobalTracerHolder::get()->beginCurrentTransaction($name, $type, $timestamp, $distributedTracingData);
    }

    /**
     * Begins a new transaction, sets as the current transaction,
     * runs the provided callback as the new transaction and automatically ends the new transaction.
     *
     * @param string                      $name      New transaction's name
     * @param string                      $type      New transaction's type
     * @param Closure                     $callback  Callback to execute as the new transaction
     * @param float|null                  $timestamp Start time of the new transaction
     * @param DistributedTracingData|null $distributedTracingData
     *
     * @return mixed The return value of $callback
     *
     * @template        T
     * @phpstan-param   Closure(TransactionInterface $newTransaction): T $callback
     * @phpstan-return  T
     *
     * @see             TransactionInterface::setName() For the description.
     * @see             TransactionInterface::setType() For the description.
     * @see             TransactionInterface::getTimestamp() For the description.
     */
    public static function captureCurrentTransaction(
        string $name,
        string $type,
        Closure $callback,
        ?float $timestamp = null,
        ?DistributedTracingData $distributedTracingData = null
    ) {
        return GlobalTracerHolder::get()->captureCurrentTransaction(
            $name,
            $type,
            $callback,
            $timestamp,
            $distributedTracingData
        );
    }

    /**
     * Returns the current transaction.
     *
     * @return TransactionInterface The current transaction
     */
    public static function getCurrentTransaction(): TransactionInterface
    {
        return GlobalTracerHolder::get()->getCurrentTransaction();
    }

    /**
     * Begins a new transaction.
     *
     * @param string                      $name      New transaction's name
     * @param string                      $type      New transaction's type
     * @param float|null                  $timestamp Start time of the new transaction
     * @param DistributedTracingData|null $distributedTracingData
     *
     * @return TransactionInterface New transaction
     *
     * @see TransactionInterface::setName() For the description.
     * @see TransactionInterface::setType() For the description.
     * @see TransactionInterface::getTimestamp() For the description.
     *
     */
    public static function beginTransaction(
        string $name,
        string $type,
        ?float $timestamp = null,
        ?DistributedTracingData $distributedTracingData = null
    ): TransactionInterface {
        return GlobalTracerHolder::get()->beginTransaction($name, $type, $timestamp, $distributedTracingData);
    }

    /**
     * Begins a new transaction,
     * runs the provided callback as the new transaction and automatically ends the new transaction.
     *
     * @param string                      $name      New transaction's name
     * @param string                      $type      New transaction's type
     * @param Closure                     $callback  Callback to execute as the new transaction
     * @param float|null                  $timestamp Start time of the new transaction
     * @param DistributedTracingData|null $distributedTracingData
     *
     * @return mixed The return value of $callback
     *
     * @template        T
     * @phpstan-param   Closure(TransactionInterface $newTransaction): T $callback
     * @phpstan-return  T
     *
     * @see             TransactionInterface::setName() For the description.
     * @see             TransactionInterface::setType() For the description.
     * @see             TransactionInterface::getTimestamp() For the description.
     */
    public static function captureTransaction(
        string $name,
        string $type,
        Closure $callback,
        ?float $timestamp = null,
        ?DistributedTracingData $distributedTracingData = null
    ) {
        return GlobalTracerHolder::get()->captureTransaction(
            $name,
            $type,
            $callback,
            $timestamp,
            $distributedTracingData
        );
    }

    /**
     * Creates an error based on the given Throwable instance
     * with the current execution segment (if there is one) as the parent.
     *
     * @param Throwable $throwable
     *
     * @return string|null ID of the reported error event or null if no event was reported
     *                      (for example, because recording is disabled)
     *
     * @link https://github.com/elastic/apm-server/blob/7.0/docs/spec/errors/error.json
     */
    public static function createError(Throwable $throwable): ?string
    {
        return GlobalTracerHolder::get()->createError($throwable);
    }

    /**
     * Pauses recording
     */
    public static function pauseRecording(): void
    {
        GlobalTracerHolder::get()->pauseRecording();
    }

    /**
     * Resumes recording
     */
    public static function resumeRecording(): void
    {
        GlobalTracerHolder::get()->resumeRecording();
    }
}
