<?php

declare(strict_types=1);

namespace Elastic\Apm\Impl\Log;

use Elastic\Apm\Impl\Util\StaticClassTrait;

/**
 * Code in this file is part of implementation internals and thus it is not covered by the backward compatibility.
 *
 * @internal
 */
final class LogCategory
{
    use StaticClassTrait;

    public const AUTO_INSTRUMENTATION = 'Auto-Instrumentation';
    public const BACKEND_COMM = 'Backend-Comm';
    public const CONFIGURATION = 'Configuration';
    public const DISCOVERY = 'Discovery';
    public const DISTRIBUTED_TRACING = 'Distributed-Tracing';
    public const INTERCEPTION = 'Interception';
    public const PUBLIC_API = 'Public-API';
}
